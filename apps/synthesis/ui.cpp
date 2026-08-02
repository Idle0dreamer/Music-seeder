#include "mq/kernel/maqam/Catalog.hpp"

#include <QApplication>
#include <QComboBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPaintEvent>
#include <QPolygonF>
#include <QPushButton>
#include <QProcess>
#include <QStandardPaths>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include <algorithm>
#include <charconv>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <expected>
#include <iterator>
#include <numbers>
#include <string>
#include <string_view>
#include <vector>

namespace {

struct AudioBuffer {
    std::vector<float> samples;
    std::uint32_t sample_rate{};
};

std::expected<AudioBuffer, std::string> load_wav(
    const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        return std::unexpected("Cannot read rendered WAV: " + path);
    }
    const std::vector<std::uint8_t> bytes(
        std::istreambuf_iterator<char>(input), {});
    if (bytes.size() < 44 ||
        std::string(bytes.begin(), bytes.begin() + 4) != "RIFF" ||
        std::string(bytes.begin() + 8, bytes.begin() + 12) != "WAVE") {
        return std::unexpected("Rendered output is not a RIFF/WAVE file.");
    }
    auto u16 = [&bytes](std::size_t offset) {
        return static_cast<std::uint16_t>(bytes[offset]) |
               (static_cast<std::uint16_t>(bytes[offset + 1]) << 8);
    };
    auto u32 = [&bytes](std::size_t offset) {
        return static_cast<std::uint32_t>(bytes[offset]) |
               (static_cast<std::uint32_t>(bytes[offset + 1]) << 8) |
               (static_cast<std::uint32_t>(bytes[offset + 2]) << 16) |
               (static_cast<std::uint32_t>(bytes[offset + 3]) << 24);
    };
    std::uint16_t channels = 0;
    std::uint16_t bits = 0;
    std::uint16_t format = 0;
    std::uint32_t sample_rate = 0;
    std::size_t data_offset = 0;
    std::size_t data_size = 0;
    std::size_t offset = 12;
    while (offset + 8 <= bytes.size()) {
        const std::string chunk(bytes.begin() + offset,
                                bytes.begin() + offset + 4);
        const auto size = static_cast<std::size_t>(u32(offset + 4));
        offset += 8;
        if (offset + size > bytes.size()) {
            return std::unexpected("WAV chunk extends beyond the file.");
        }
        if (chunk == "fmt " && size >= 16) {
            format = u16(offset);
            channels = u16(offset + 2);
            sample_rate = u32(offset + 4);
            bits = u16(offset + 14);
        } else if (chunk == "data") {
            data_offset = offset;
            data_size = size;
        }
        offset += size + (size % 2);
    }
    if (format != 1 || channels == 0 || bits != 16 || sample_rate == 0 ||
        data_size == 0) {
        return std::unexpected(
            "The player view requires a non-empty mono/stereo 16-bit PCM WAV.");
    }
    const auto frame_bytes = static_cast<std::size_t>(channels) * 2;
    const auto frames = data_size / frame_bytes;
    AudioBuffer result;
    result.sample_rate = sample_rate;
    result.samples.reserve(frames);
    for (std::size_t frame = 0; frame < frames; ++frame) {
        const auto sample_offset = data_offset + frame * frame_bytes;
        const auto value = static_cast<std::int16_t>(
            static_cast<std::uint16_t>(bytes[sample_offset]) |
            (static_cast<std::uint16_t>(bytes[sample_offset + 1]) << 8));
        result.samples.push_back(static_cast<float>(value) / 32768.0F);
    }
    return result;
}

enum class ViewKind {
    Waveform,
    Spectrum,
};

class SignalView final : public QWidget {
public:
    explicit SignalView(ViewKind kind, QWidget* parent = nullptr)
        : QWidget(parent), kind_(kind) {
        setMinimumSize(320, 150);
        setAutoFillBackground(false);
    }

    void set_audio(const AudioBuffer& audio) {
        samples_ = audio.samples;
        sample_rate_ = audio.sample_rate;
        update();
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(24, 27, 32));
        const QRectF plot(12.0, 10.0, width() - 24.0, height() - 22.0);
        painter.setPen(QPen(QColor(90, 98, 110), 1.0));
        painter.drawRect(plot);
        if (samples_.empty()) {
            painter.setPen(QColor(170, 178, 190));
            painter.drawText(plot, Qt::AlignCenter, "Generate a WAV to view it");
            return;
        }
        if (kind_ == ViewKind::Waveform) {
            paint_waveform(painter, plot);
        } else {
            paint_spectrum(painter, plot);
        }
    }

private:
    void paint_waveform(QPainter& painter, const QRectF& plot) const {
        painter.setPen(QPen(QColor(92, 104, 118), 1.0));
        painter.drawLine(plot.left(), plot.center().y(), plot.right(),
                         plot.center().y());
        QPolygonF points;
        points.reserve(static_cast<int>(plot.width()));
        for (int x = 0; x < static_cast<int>(plot.width()); ++x) {
            const auto fraction = plot.width() <= 1.0
                                      ? 0.0
                                      : static_cast<double>(x) /
                                            (plot.width() - 1.0);
            const auto index = static_cast<std::size_t>(
                fraction * static_cast<double>(samples_.size() - 1));
            points.append(QPointF(
                plot.left() + x,
                plot.center().y() - samples_[index] * plot.height() * 0.45));
        }
        painter.setPen(QPen(QColor(77, 202, 170), 1.2));
        painter.drawPolyline(points);
        painter.setPen(QColor(170, 178, 190));
        painter.drawText(plot.left() + 6, plot.top() + 16,
                         QString("WAV waveform  %1 Hz")
                             .arg(sample_rate_));
    }

    void paint_spectrum(QPainter& painter, const QRectF& plot) const {
        constexpr std::size_t window = 1024;
        const auto count = std::min(window, samples_.size());
        if (count < 4) {
            return;
        }
        std::vector<double> magnitude(count / 2);
        double maximum = 0.0;
        for (std::size_t bin = 0; bin < magnitude.size(); ++bin) {
            double real = 0.0;
            double imaginary = 0.0;
            for (std::size_t sample = 0; sample < count; ++sample) {
                const auto window_value =
                    0.5 - 0.5 * std::cos(
                              2.0 * std::numbers::pi * sample /
                              static_cast<double>(count - 1));
                const auto angle = 2.0 * std::numbers::pi * bin * sample /
                                   static_cast<double>(count);
                real += samples_[sample] * window_value * std::cos(angle);
                imaginary -=
                    samples_[sample] * window_value * std::sin(angle);
            }
            magnitude[bin] = std::sqrt(real * real + imaginary * imaginary);
            maximum = std::max(maximum, magnitude[bin]);
        }
        if (maximum <= 0.0) {
            return;
        }
        QPolygonF points;
        points.reserve(static_cast<int>(plot.width()));
        for (int x = 0; x < static_cast<int>(plot.width()); ++x) {
            const auto fraction = plot.width() <= 1.0
                                      ? 0.0
                                      : static_cast<double>(x) /
                                            (plot.width() - 1.0);
            const auto bin = std::min(
                magnitude.size() - 1,
                static_cast<std::size_t>(fraction * magnitude.size()));
            const auto db = 20.0 * std::log10(
                std::max(magnitude[bin] / maximum, 1.0e-5));
            const auto normalized = std::clamp((db + 100.0) / 100.0, 0.0, 1.0);
            points.append(QPointF(
                plot.left() + x,
                plot.bottom() - normalized * (plot.height() - 10.0)));
        }
        painter.setPen(QPen(QColor(246, 178, 84), 1.2));
        painter.drawPolyline(points);
        painter.setPen(QColor(170, 178, 190));
        painter.drawText(plot.left() + 6, plot.top() + 16,
                         QString("Soft Fourier magnitude  0-%1 Hz")
                             .arg(sample_rate_ / 2));
    }

    ViewKind kind_;
    std::vector<float> samples_;
    std::uint32_t sample_rate_{};
};

class PlayerWindow final : public QWidget {
public:
    PlayerWindow() {
        setWindowTitle("Music-seeder Generator / Player");
        resize(760, 720);

        auto* outer = new QVBoxLayout(this);
        auto* heading = new QLabel(
            "Generate a deterministic instrument-neutral maqam performance.",
            this);
        heading->setWordWrap(true);
        outer->addWidget(heading);

        auto* options = new QGroupBox("Performance configuration", this);
        auto* form = new QFormLayout(options);

        maqam_ = new QComboBox(options);
        const auto catalog = mq::kernel::maqam::Catalog::declared();
        for (const auto& entry : catalog.entries()) {
            if (!entry.specification && entry.kind != "legacy") {
                continue;
            }
            maqam_->addItem(
                QString::fromStdString(entry.name),
                QString::fromStdString(entry.name));
        }
        const auto rast = maqam_->findData("rast");
        if (rast >= 0) {
            maqam_->setCurrentIndex(rast);
        }
        form->addRow("Maqam", maqam_);

        seed_ = new QLineEdit("1", options);
        seed_->setPlaceholderText("unsigned seed");
        form->addRow("Seed", seed_);

        timing_ = new QComboBox(options);
        for (const auto& path : timing_profiles()) {
            timing_->addItem(QString::fromStdString(path),
                             QString::fromStdString(path));
        }
        form->addRow("Timing profile", timing_);

        tonic_ = new QDoubleSpinBox(options);
        tonic_->setRange(20.0, 2'000.0);
        tonic_->setDecimals(8);
        tonic_->setSingleStep(1.0);
        tonic_->setValue(146.8323839587);
        tonic_->setSuffix(" Hz");
        form->addRow("Tonic", tonic_);

        sample_rate_ = new QComboBox(options);
        sample_rate_->addItem("44100 Hz", 44'100);
        sample_rate_->addItem("48000 Hz", 48'000);
        sample_rate_->addItem("96000 Hz", 96'000);
        sample_rate_->setCurrentIndex(1);
        form->addRow("Sample rate", sample_rate_);

        duration_ = new QDoubleSpinBox(options);
        duration_->setRange(3.0, 90.0);
        duration_->setDecimals(1);
        duration_->setSingleStep(1.0);
        duration_->setValue(30.0);
        duration_->setSuffix(" seconds");
        form->addRow("Performance length", duration_);

        auto* output_row = new QWidget(options);
        auto* output_layout = new QHBoxLayout(output_row);
        output_layout->setContentsMargins(0, 0, 0, 0);
        output_ = new QLineEdit("build/music-seed.wav", output_row);
        auto* browse = new QPushButton("Browse...", output_row);
        output_layout->addWidget(output_);
        output_layout->addWidget(browse);
        form->addRow("WAV output", output_row);
        outer->addWidget(options);

        auto* controls = new QHBoxLayout();
        generate_ = new QPushButton("Generate", this);
        play_ = new QPushButton("Play", this);
        stop_ = new QPushButton("Stop", this);
        play_->setEnabled(false);
        stop_->setEnabled(false);
        controls->addWidget(generate_);
        controls->addWidget(play_);
        controls->addWidget(stop_);
        loop_ = new QCheckBox("Loop continuously", this);
        controls->addWidget(loop_);
        controls->addStretch();
        outer->addLayout(controls);

        auto* views = new QHBoxLayout();
        auto* waveform_box = new QGroupBox("WAV view", this);
        auto* waveform_layout = new QVBoxLayout(waveform_box);
        waveform_ = new SignalView(ViewKind::Waveform, waveform_box);
        waveform_layout->addWidget(waveform_);
        auto* spectrum_box = new QGroupBox("Fourier view", this);
        auto* spectrum_layout = new QVBoxLayout(spectrum_box);
        spectrum_ = new SignalView(ViewKind::Spectrum, spectrum_box);
        spectrum_layout->addWidget(spectrum_);
        views->addWidget(waveform_box);
        views->addWidget(spectrum_box);
        outer->addLayout(views, 1);

        status_ = new QLabel("Ready.", this);
        status_->setWordWrap(true);
        outer->addWidget(status_);
        outer->addStretch();

        connect(browse, &QPushButton::clicked, this, [this] {
            const auto path = QFileDialog::getSaveFileName(
                this,
                "Choose WAV output",
                output_->text(),
                "WAV files (*.wav);;All files (*)");
            if (!path.isEmpty()) {
                output_->setText(path);
            }
        });
        connect(generate_, &QPushButton::clicked, this, [this] {
            generate_audio();
        });
        connect(play_, &QPushButton::clicked, this, [this] {
            play_audio();
        });
        connect(stop_, &QPushButton::clicked, this, [this] {
            stopping_ = true;
            if (generator_ != nullptr) {
                generator_->kill();
            }
            if (player_ != nullptr) {
                player_->kill();
            }
        });
    }

private:
    static std::vector<std::string> timing_profiles() {
        std::vector<std::string> result;
        const std::filesystem::path directory =
            "theory/data/performance";
        std::error_code error;
        for (const auto& item :
             std::filesystem::directory_iterator(directory, error)) {
            if (item.path().extension() == ".timing") {
                result.push_back(item.path().string());
            }
        }
        std::ranges::sort(result);
        if (result.empty()) {
            result.push_back(
                "theory/data/performance/free-rhythm-v1.timing");
        }
        return result;
    }

    std::expected<std::uint64_t, std::string> seed_value() const {
        std::uint64_t result{};
        const auto value = seed_->text().toStdString();
        const auto parsed = std::from_chars(
            value.data(), value.data() + value.size(), result);
        if (parsed.ec != std::errc{} ||
            parsed.ptr != value.data() + value.size()) {
            return std::unexpected("Seed must be an unsigned integer.");
        }
        return result;
    }

    void generate_audio() {
        if (generator_ != nullptr) {
            return;
        }
        const auto seed = seed_value();
        if (!seed) {
            show_error(seed.error());
            return;
        }
        const auto path = output_->text().toStdString();
        if (path.empty()) {
            show_error("Choose a WAV output path.");
            return;
        }
        const std::filesystem::path output_path(path);
        if (!output_path.parent_path().empty()) {
            std::error_code error;
            std::filesystem::create_directories(output_path.parent_path(), error);
            if (error) {
                show_error("Cannot create output directory: " + error.message());
                return;
            }
        }
        const auto local_renderer =
            std::filesystem::path(QApplication::applicationDirPath().toStdString()) /
            "synthesis-render";
        QString executable;
        if (std::filesystem::exists(local_renderer)) {
            executable = QString::fromStdString(local_renderer.string());
        } else {
            executable = QStandardPaths::findExecutable("synthesis-render");
        }
        if (executable.isEmpty()) {
            show_error(
                "The synthesis renderer was not found beside the player.");
            return;
        }

        stopping_ = false;
        generate_->setEnabled(false);
        play_->setEnabled(false);
        stop_->setEnabled(true);
        status_->setText("Generating the continuous performance...");
        generator_ = new QProcess(this);
        const QString generated_path = QString::fromStdString(path);
        const QStringList arguments{
            QStringLiteral("--maqam"),
            maqam_->currentData().toString(),
            QStringLiteral("--seed"),
            QString::number(static_cast<qulonglong>(*seed)),
            QStringLiteral("--timing"),
            timing_->currentData().toString(),
            QStringLiteral("--sample-rate"),
            QString::number(sample_rate_->currentData().toUInt()),
            QStringLiteral("--tonic-hz"),
            QString::number(tonic_->value(), 'g', 15),
            QStringLiteral("--duration"),
            QString::number(duration_->value(), 'f', 1),
            QStringLiteral("--output"),
            generated_path,
        };
        generator_->start(executable, arguments);
        connect(generator_,
                qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
                this,
                [this, generated_path](
                    int exitCode, QProcess::ExitStatus exitStatus) {
            const auto error = QString::fromLocal8Bit(
                                   generator_->readAllStandardError())
                                   .trimmed();
            auto* completed = generator_;
            generator_ = nullptr;
            completed->deleteLater();
            generate_->setEnabled(true);
            stop_->setEnabled(false);
            if (stopping_) {
                stopping_ = false;
                play_after_generation_ = false;
                status_->setText("Generation stopped.");
                return;
            }
            if (exitStatus != QProcess::NormalExit || exitCode != 0) {
                play_after_generation_ = false;
                show_error(error.isEmpty()
                               ? "The synthesis renderer failed."
                               : error.toStdString());
                return;
            }
            last_output_ = generated_path;
            const auto audio = load_wav(last_output_.toStdString());
            if (!audio) {
                play_after_generation_ = false;
                show_error(audio.error());
                return;
            }
            waveform_->set_audio(*audio);
            spectrum_->set_audio(*audio);
            play_->setEnabled(true);
            status_->setText(
                QString("Generated %1 frames (%2 seconds).\n%3")
                    .arg(audio->samples.size())
                    .arg(static_cast<double>(audio->samples.size()) /
                             static_cast<double>(audio->sample_rate),
                         0,
                         'f',
                         1)
                    .arg(last_output_));
            if (play_after_generation_) {
                play_after_generation_ = false;
                stopping_ = false;
                start_playback();
            }
        });
    }

    void play_audio() {
        if (generator_ != nullptr) {
            play_after_generation_ = true;
            return;
        }
        if (last_output_.isEmpty() ||
            !std::filesystem::exists(last_output_.toStdString())) {
            play_after_generation_ = true;
            generate_audio();
            return;
        }
        stopping_ = false;
        start_playback();
    }

    void start_playback() {
        if (player_ != nullptr) {
            player_->kill();
            player_->deleteLater();
            player_ = nullptr;
        }
        QString executable;
        for (const auto& candidate : {QStringLiteral("paplay"),
                                      QStringLiteral("aplay"),
                                      QStringLiteral("ffplay")}) {
            executable = QStandardPaths::findExecutable(candidate);
            if (!executable.isEmpty()) {
                break;
            }
        }
        if (executable.isEmpty()) {
            show_error("No desktop audio playback command was found.");
            return;
        }
        player_ = new QProcess(this);
        const QStringList arguments =
            executable.endsWith("ffplay")
                ? QStringList{QStringLiteral("-nodisp"),
                              QStringLiteral("-autoexit"), last_output_}
                : QStringList{last_output_};
        player_->start(executable, arguments);
        connect(player_,
                qOverload<int, QProcess::ExitStatus>(&QProcess::finished),
                this,
                [this](int, QProcess::ExitStatus) {
            stop_->setEnabled(false);
            const bool repeat = loop_->isChecked() && !stopping_;
            player_->deleteLater();
            player_ = nullptr;
            if (repeat) {
                QTimer::singleShot(0, this, [this] { start_playback(); });
            }
        });
        connect(player_, &QProcess::started, this, [this] {
            stop_->setEnabled(true);
            status_->setText("Playing " + last_output_);
        });
        connect(player_, &QProcess::errorOccurred, this, [this] {
            show_error("Audio playback could not be started.");
        });
    }

    void show_error(const std::string& message) {
        status_->setText(QString::fromStdString(message));
        QMessageBox::critical(
            this, "Music-seeder", QString::fromStdString(message));
    }

    QComboBox* maqam_{};
    QLineEdit* seed_{};
    QComboBox* timing_{};
    QDoubleSpinBox* tonic_{};
    QComboBox* sample_rate_{};
    QDoubleSpinBox* duration_{};
    QLineEdit* output_{};
    SignalView* waveform_{};
    SignalView* spectrum_{};
    QPushButton* generate_{};
    QPushButton* play_{};
    QPushButton* stop_{};
    QCheckBox* loop_{};
    QLabel* status_{};
    QProcess* generator_{};
    QProcess* player_{};
    bool stopping_{};
    bool play_after_generation_{};
    QString last_output_;
};

} // namespace

int main(int argc, char** argv) {
    QApplication application(argc, argv);
    if (argc == 2 && std::string_view(argv[1]) == "--self-test") {
        const auto catalog = mq::kernel::maqam::Catalog::declared();
        return catalog.entries().empty() ? 1 : 0;
    }
    PlayerWindow window;
    window.show();
    return application.exec();
}

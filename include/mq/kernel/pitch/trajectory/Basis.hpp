#pragma once

#include <cmath>
#include <variant>

namespace mq::kernel::pitch::trajectory {

struct Basis {
    struct Hertz { double value; };
    struct Cents { double value; };

    struct Stable {};
    
    struct Onset {
        double duration; // fraction of total time [0, 1]
    };
    
    struct Release {
        double duration; // fraction of total time [0, 1]
    };
    
    struct Vibrato {
        Hertz rate;
        Cents extent;
    };

    struct Attraction {
        Cents amount;
        double skew;
    };

    using Form = std::variant<Stable, Onset, Release, Vibrato, Attraction>;
    Form form;

    [[nodiscard]] double evaluate(double t) const noexcept;
};

} // namespace mq::kernel::pitch::trajectory

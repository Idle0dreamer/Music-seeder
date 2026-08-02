#pragma once

#include "mq/kernel/performance/TimedEvent.hpp"
#include "mq/kernel/performance/Pause.hpp"

#include <algorithm>
#include <optional>
#include <utility>
#include <vector>

namespace mq::kernel::performance {

struct Plan {
    std::vector<TimedEvent> events;
    std::vector<Pause> pauses;

    [[nodiscard]] Rational end() const {
        Rational result{};
        for (const auto& event : events) {
            result = std::max(result, event.onset + event.duration);
        }
        for (const auto& pause : pauses) {
            result = std::max(result, pause.onset + pause.duration);
        }
        return result;
    }

    [[nodiscard]] bool well_formed() const {
        Rational expected{};
        std::size_t pause_index{};
        sort::StrandId active_strand;
        bool has_strand = false;
        for (const auto& event : events) {
            while (pause_index < pauses.size() &&
                   pauses[pause_index].onset == expected) {
                if (!pauses[pause_index].well_formed()) return false;
                expected += pauses[pause_index].duration;
                ++pause_index;
            }
            if (!event.well_formed() || event.onset != expected ||
                (has_strand && event.strand != active_strand)) {
                return false;
            }
            if (!has_strand) {
                active_strand = event.strand;
                has_strand = true;
            }
            expected += event.duration;
        }
        while (pause_index < pauses.size() &&
               pauses[pause_index].onset == expected) {
            if (!pauses[pause_index].well_formed()) return false;
            expected += pauses[pause_index].duration;
            ++pause_index;
        }
        return pause_index == pauses.size();
    }

    void append(
        Target target,
        Rational duration,
        Rational intensity,
        Articulation articulation,
        sort::StrandId strand = monophonic(),
        std::optional<Release> release = std::nullopt) {
        events.push_back({
            std::move(target),
            end(),
            duration,
            intensity,
            articulation,
            std::move(strand),
            std::nullopt,
            std::nullopt,
            std::move(release),
        });
    }

    void append_pause(
        Rational duration,
        Identity function,
        std::string provenance) {
        pauses.push_back({end(), duration, std::move(function),
                          std::move(provenance)});
    }
};

} // namespace mq::kernel::performance

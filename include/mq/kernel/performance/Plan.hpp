#pragma once

#include "mq/kernel/performance/TimedEvent.hpp"

#include <utility>
#include <vector>

namespace mq::kernel::performance {

struct Plan {
    std::vector<TimedEvent> events;

    [[nodiscard]] Rational end() const {
        if (events.empty()) {
            return Rational(0);
        }
        const auto& last = events.back();
        return last.onset + last.duration;
    }

    [[nodiscard]] bool well_formed() const {
        Rational expected{};
        sort::StrandId active_strand;
        bool has_strand = false;
        for (const auto& event : events) {
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
        return true;
    }

    void append(
        Target target,
        Rational duration,
        Rational intensity,
        Articulation articulation,
        sort::StrandId strand = monophonic()) {
        events.push_back({
            std::move(target),
            end(),
            duration,
            intensity,
            articulation,
            std::move(strand),
            std::nullopt,
            std::nullopt,
        });
    }
};

} // namespace mq::kernel::performance

import os
import re
from pathlib import Path

# Mapping of file -> list of (regex, replacement, include_to_add)
replacements = {
    "include/mq/kernel/operation/Operation.hpp": [
        (r"Identity center;", "sort::CenterId center;", "mq/kernel/sort/CenterId.hpp"),
        (r"Identity jins;", "sort::JinsId jins;", "mq/kernel/sort/JinsId.hpp"),
        (r"Identity role;", "sort::RoleId role;", "mq/kernel/sort/RoleId.hpp"),
        (r"Identity cell;", "sort::CellId cell;", "mq/kernel/sort/CellId.hpp"),
        (r"Identity family;", "sort::FamilyId family;", "mq/kernel/sort/FamilyId.hpp"),
        (r"Identity path;", "sort::PathId path;", "mq/kernel/sort/PathId.hpp"),
        (r"Identity event;", "sort::EventId event;", "mq/kernel/sort/EventId.hpp"),
        (r"Identity region;", "sort::RegionId region;", "mq/kernel/sort/RegionId.hpp"),
        (r"std::optional<Identity> baggage;", "std::optional<sort::BaggageId> baggage;", "mq/kernel/sort/BaggageId.hpp"),
        (r"Identity phrase;", "sort::PhraseId phrase;", "mq/kernel/sort/PhraseId.hpp"),
    ],
    "include/mq/kernel/state/Snapshot.hpp": [
        (r"std::vector<Identity> stack;", "std::vector<sort::CenterId> stack;", "mq/kernel/sort/CenterId.hpp"),
        (r"std::optional<Identity> active;", "std::optional<sort::JinsId> active;", "mq/kernel/sort/JinsId.hpp"),
        (r"std::map<Identity, std::size_t> occurrences;", "std::map<sort::CellId, std::size_t> occurrences;", "mq/kernel/sort/CellId.hpp"),
        (r"std::set<Identity> completed;", "std::set<sort::PathId> completed;", "mq/kernel/sort/PathId.hpp"),
        (r"std::set<Identity> completed; // Sayr", "std::set<sort::ObligationId> completed;", "mq/kernel/sort/ObligationId.hpp"),
        (r"std::map<Identity, std::size_t> budget;", "std::map<sort::ProductionId, std::size_t> budget;", "mq/kernel/sort/ProductionId.hpp"),
    ]
}

def process_file(filepath):
    if filepath not in replacements: return
    p = Path(filepath)
    if not p.exists(): return
    content = p.read_text()
    
    includes = set()
    for pattern, repl, inc in replacements[filepath]:
        if re.search(pattern, content):
            content = re.sub(pattern, repl, content)
            includes.add(inc)
            
    if includes:
        lines = content.split('\n')
        # find last include
        last_inc = -1
        for i, line in enumerate(lines):
            if line.startswith('#include '): last_inc = i
        
        inc_str = "\n".join(f'#include "{inc}"' for inc in sorted(includes))
        if last_inc != -1:
            lines.insert(last_inc + 1, inc_str)
        else:
            lines.insert(0, inc_str)
        p.write_text('\n'.join(lines))

for f in replacements:
    process_file(f)

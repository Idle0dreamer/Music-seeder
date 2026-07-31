import os, glob, re

replacements = {
    # operation/Operation.hpp done
    # state/Snapshot.hpp done
    
    # performance/Event.hpp
    r'Identity identity;': r'sort::EventId identity;',
    r'Identity role;': r'sort::RoleId role;',
    r'Identity region;': r'sort::RegionId region;',
    r'std::optional<Identity> baggage;': r'std::optional<sort::BaggageId> baggage;',
    
    # performance/Gesture.hpp
    r'Identity occurrence;': r'sort::OccurrenceId occurrence;',
    r'Identity family;': r'sort::FamilyId family;',
    
    # jins/Descriptor.hpp
    r'Identity tonic;': r'sort::RoleId tonic;',
    r'std::vector<Identity> span;': r'std::vector<sort::RoleId> span;',
    r'std::vector<Identity> ghammaz;': r'std::vector<sort::RoleId> ghammaz;',
    r'std::vector<Identity> register_low;': r'std::vector<sort::RegionId> register_low;',
    r'std::vector<Identity> register_high;': r'std::vector<sort::RegionId> register_high;',
    r'std::vector<Identity> emphasis;': r'std::vector<sort::RoleId> emphasis;',
    
    # jins/Overlay.hpp
    r'Identity identity;': r'sort::JinsId identity;',
    # a bunch of RoleId and RegionId inside Overlay.hpp, need better regex
}

def fix_allows():
    for f in glob.glob('src/kernel/eval/**/*.cpp', recursive=True):
        if not os.path.isfile(f): continue
        c = open(f).read()
        c = re.sub(r'allows\(([^,]+),\s*([a-zA-Z_]+)\.(center|jins|role|cell|family|path|event|region|baggage|phrase)\)', r'allows(\1, \2.\3.identity)', c)
        c = re.sub(r'allows\(([^,]+),\s*\*([a-zA-Z_]+)\.baggage\)', r'allows(\1, \2.baggage->identity)', c)
        open(f, 'w').write(c)

fix_allows()

import re

with open('src/kernel/generate/Candidate.cpp', 'r') as fp:
    lines = fp.readlines()

for i in range(len(lines)):
    if 'history == std::set<Identity>(state.sayr.completed.begin(), state.sayr.completed.end())' in lines[i]:
        # We need to construct set<Identity> properly. TypedId has no implicit conversion to Identity, so we must map it.
        lines[i] = lines[i].replace('history == std::set<Identity>(state.sayr.completed.begin(), state.sayr.completed.end())', 'true /* sayr path completed check removed temporarily, fix properly */')
        # Actually, let's fix it properly using a lambda or just a loop.
        # wait, we can't inject a loop easily in a boolean expression.
        # Let's write a small helper lambda:
        lines[i] = """            [&]() { 
                std::set<Identity> s; 
                for (auto& id : state.sayr.completed) s.insert(id.identity); 
                return history == s; 
            }()
"""
        break

with open('src/kernel/generate/Candidate.cpp', 'w') as fp:
    fp.writelines(lines)

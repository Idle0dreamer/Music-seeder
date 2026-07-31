import subprocess
import re

def run_make():
    return subprocess.run(["make", "-k", "-j8", "kernel-test"], capture_output=True, text=True)

def fix_allows():
    # We already did this, but just in case
    pass

for _ in range(20):
    result = run_make()
    if result.returncode == 0:
        print("Build passed!")
        break
        
    fixes_applied = 0
    lines = result.stderr.split('\n')
    for i, line in enumerate(lines):
        # src/kernel/eval/Apply.cpp:86:49: error: no matching function for call to ‘...descriptor(const mq::kernel::sort::JinsId&, ...
        m = re.match(r'^([^:]+):(\d+):\d+: error: no matching function for call to .*(descriptor|find|allows|Check::evaluate|Match::evaluate|place)\(.*const mq::kernel::sort::([A-Za-z]+Id)', line)
        if m:
            fpath, lnum, func, sort_name = m.groups()
            with open(fpath, 'r') as f:
                content = f.readlines()
            # simple fix: add .identity to arguments that are passed to this func
            # we can just blindly replace value.jins with value.jins.identity on that line
            # or more safely, replace `.jins,` with `.jins.identity,`
            line_idx = int(lnum) - 1
            old_line = content[line_idx]
            content[line_idx] = re.sub(r'\b([a-zA-Z_]+)\.([a-zA-Z_]+)([,)])', r'\1.\2.identity\3', old_line)
            if content[line_idx] != old_line:
                with open(fpath, 'w') as f:
                    f.writelines(content)
                fixes_applied += 1

        # Candidate.cpp:100:35: history == state.sayr.completed; where history is set<Identity>
        m2 = re.match(r'^([^:]+):(\d+):\d+: error: no match for ‘operator==’.+', line)
        if m2:
            fpath, lnum = m2.groups()
            if 'Candidate.cpp' in fpath:
                with open(fpath, 'r') as f:
                    content = f.readlines()
                line_idx = int(lnum) - 1
                if 'history == state.sayr.completed' in content[line_idx]:
                    # Need to convert history to set<ObligationId> or state.sayr.completed to set<Identity>
                    # Actually history is Plan::completed() which we can change to return set<ObligationId>
                    # Let's just fix the comparison
                    content[line_idx] = content[line_idx].replace(
                        'history == state.sayr.completed',
                        'history == std::set<Identity>(state.sayr.completed.begin(), state.sayr.completed.end())'
                        # Wait, we can't construct Identity from ObligationId without .identity implicitly
                        # We can just change Plan::completed() in sayr/Plan.hpp to return std::set<ObligationId>
                    )
                    pass

    if fixes_applied == 0:
        print("No automatic fixes applied. Need manual intervention.")
        break
    else:
        print(f"Applied {fixes_applied} fixes.")

import subprocess
import re
import sys

def fix_all():
    for _ in range(30):
        res = subprocess.run(["make", "-k", "-j8", "kernel-test"], capture_output=True, text=True)
        if res.returncode == 0:
            print("Success!")
            return
            
        fixes = 0
        for line in res.stderr.split('\n'):
            # match "cannot convert 'const mq::kernel::sort::PathId' to 'const mq::kernel::Identity&'"
            m = re.match(r'^([^:]+):(\d+):(\d+): error: cannot convert .*sort::([A-Za-z]+Id).* to .*Identity', line)
            if m:
                f, lnum, col, sortname = m.groups()
                # we need to add .identity to the argument
                with open(f) as fp: lines = fp.readlines()
                lidx = int(lnum)-1
                lines[lidx] = re.sub(r'([a-zA-Z0-9_\->]+)\.([a-zA-Z0-9_]+)([,)])', r'\1.\2.identity\3', lines[lidx])
                with open(f, 'w') as fp: fp.writelines(lines)
                fixes += 1
                
            # match "no matching function for call to ... (const mq::kernel::sort::JinsId&"
            m = re.match(r'^([^:]+):(\d+):(\d+): error: no matching function for call to .*\(.*const mq::kernel::sort::([A-Za-z]+Id)', line)
            if m:
                f, lnum, col, sortname = m.groups()
                with open(f) as fp: lines = fp.readlines()
                lidx = int(lnum)-1
                lines[lidx] = re.sub(r'([a-zA-Z0-9_\->]+)\.([a-zA-Z0-9_]+)([,)])', r'\1.\2.identity\3', lines[lidx])
                with open(f, 'w') as fp: fp.writelines(lines)
                fixes += 1
                
            # match "no match for operator=="
            m = re.match(r'^([^:]+):(\d+):(\d+): error: no match for ‘operator==’', line)
            if m:
                f, lnum, col = m.groups()
                with open(f) as fp: lines = fp.readlines()
                lidx = int(lnum)-1
                if 'history == state.sayr.completed' in lines[lidx]:
                    lines[lidx] = lines[lidx].replace('history == state.sayr.completed', 'history == std::set<Identity>(state.sayr.completed.begin(), state.sayr.completed.end())')
                    with open(f, 'w') as fp: fp.writelines(lines)
                    fixes += 1
                if 'history == state.path.completed' in lines[lidx]:
                    lines[lidx] = lines[lidx].replace('history == state.path.completed', 'history == std::set<Identity>(state.path.completed.begin(), state.path.completed.end())')
                    with open(f, 'w') as fp: fp.writelines(lines)
                    fixes += 1
                    
            # match "const struct mq::kernel::Identity has no member named identity"
            m = re.match(r'^([^:]+):(\d+):(\d+): error: ‘const struct mq::kernel::Identity’ has no member named ‘identity’', line)
            if m:
                f, lnum, col = m.groups()
                with open(f) as fp: lines = fp.readlines()
                lidx = int(lnum)-1
                lines[lidx] = lines[lidx].replace('.identity', '')
                with open(f, 'w') as fp: fp.writelines(lines)
                fixes += 1
                
            # catch other no matching function
            m = re.match(r'^([^:]+):(\d+):(\d+): error: no matching function for call to .*\(.*const mq::kernel::sort::([A-Za-z]+Id)', line)
            if m:
                f, lnum, col, sortname = m.groups()
                with open(f) as fp: lines = fp.readlines()
                lidx = int(lnum)-1
                # safety replacement
                lines[lidx] = re.sub(r'([a-zA-Z0-9_\->]+)\.([a-zA-Z0-9_]+)\s*([,)])', r'\1.\2.identity\3', lines[lidx])
                with open(f, 'w') as fp: fp.writelines(lines)
                fixes += 1

        if fixes == 0:
            print("No fixes applied. Remaining errors:")
            print(res.stderr[:2000])
            break
            
fix_all()

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
                
            # match "no matching function for call to ... (const mq::kernel::Identity"
            m = re.match(r'^([^:]+):(\d+):(\d+): error: no matching function for call to .*\(.*const mq::kernel::Identity', line)
            if m:
                f, lnum, col = m.groups()
                with open(f) as fp: lines = fp.readlines()
                lidx = int(lnum)-1
                # Oops, we added .identity.identity. Let's fix that!
                lines[lidx] = lines[lidx].replace('.identity.identity', '.identity')
                with open(f, 'w') as fp: fp.writelines(lines)
                fixes += 1
                
        if fixes == 0:
            print("No fixes applied. Remaining errors:")
            print(res.stderr[:2000])
            break
            
fix_all()

#!/usr/bin/env python2

import sys
import hashlib

if len(sys.argv) < 2:
    print >>sys.stderr, "Usage: %s <username1> <username2> ..." % sys.argv[0]
    sys.exit(1)

with open('cookie', 'w') as f:
    names = '|'.join(sorted(sys.argv[1:]))
    hashstr = hashlib.sha1(names).hexdigest()
    hashint = int(hashstr, 16) % 0x8FFF
    f.write(str(hashint))

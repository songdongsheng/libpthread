#!/usr/bin/env python
import sys

fm = {}
for f in (
        'pthread_dfly.txt',
        'pthread_freebsd.txt',
        'pthread_linux.txt',
        'pthread_netbsd.txt'):
    for line in open(f, 'r'):
        line = line.strip()
        if len(line) > 0:
            if line in fm:
                fm[line] = fm[line] + 1
            else:
                fm[line] = 1


print '#### 5 ####'
for f in fm.iteritems():
    if f[1] >= 5:
        print f[0]


print '#### 4 ####'
for f in fm.iteritems():
    if f[1] == 4:
        print f[0]


print '#### 3 ####'
for f in fm.iteritems():
    if f[1] == 3:
        print f[0]


print '#### 2 ####'
for f in fm.iteritems():
    if f[1] == 2:
        print f[0]

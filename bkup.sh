#!/bin/sh
make -C . clean
find . -type f | grep '^\..*\.' | grep -v '\.DS_Store' | grep -v '\.bk' | grep -v '\.old' | grep -v '\.dSYM' | grep -v 'Info.plist' | grep -v '\.a' | grep -v '\.git' | grep -v '\.swp$' > '$$tmpfile'
find . -type f | grep 'makefile' >> '$$tmpfile'
cat '$$tmpfile' | xargs tar cvf - | gzip -c > '../bkup/dptl_'`date '+%Y%m%d'`.tgz
rm '$$tmpfile'


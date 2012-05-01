#!/bin/bash
# When doing parallel builds the following packages often blow up
make -j 24 clean
make -j 24
make V=s
make -j 16 package/e2fsprogs/{clean,compile,install}
make -j 16 package/krb5/{clean,compile,install}
make -j 16 package/pcre/{clean,compile,install}
make -j 24

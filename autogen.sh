#!/bin/sh -x
aclocal
autoheader
automake --add-missing --copy --foreign
autoconf

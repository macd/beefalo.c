The build has been modified to only build the program beefalo and not
all the other build artifacts.

To build beefalo, a modified bison, you should only need to do a

    ./configure
    make

but if, for some reason, you need to go back and edit configure.ac
and Makefile.am, you then need to use autotools to regenerrate configure,
so do the following

    aclocal --system-acdir=m4
    autoheader
    automake --add-missing --foreign
    autoconf
    ./configure
    make

Note that we have emulated the later bison's feature of having the skeleton
file directly included in the executable.  The "raw" skeleton can be found
in **src/beefalo.simple**. That file is lightly processed into the file 
**beefskel.c** If you want to experiment with a modified skeleton file you can
use **beefalo.simple** as a starting point and then just feed that to beefalo
with -S switch. If you want to include a modified skeleton into the executable,
then you must process the skeleton file to match the format in **beefskel.c**

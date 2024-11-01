# dnsSniffer

# Some notes:
- In case we are doing cross compilation, then need to set a global debug symbol that will indicate whether the
target machine is little or big endian.

- Before deploying the software, the build script can also run some unit tests (after it builds the software).
However, it will require that later the unit test package along with all of its "left overs" will be removed.
An alternative to that is to implement the unit tests in pure "regualr functions" and just run the final 
executable with some "-test" command line --> this way there will be no need to have the unit tests framework
installed on the production target.
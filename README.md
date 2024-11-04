# dnsSniffer
Linux based that utilizes the libpcap library to sniff DNS response and maintain "in house DB" of 
addresses it encountered in the sniffed packets.

# Research notes:
- During the research phase several libraries to capture packets were encountered (such as DPDK). For ease of deployment
, easy usage and popularity reasons - the `libpcap` library was choosed.

# Development environment notes:
- The application was developed and tested on Ubuntu 20.04 VM with the following:
- Architecture: X86_64 (VM).
- Linux kernel 5.4.0
- `libpcap` that was manually installed on the VM: 1.9.1
- `libdbus` 3.19.1
- `gcc` version used: 9.4.0
- `pkg-config` version used: 0.29.1
- `cmake` version used: 3.16.3
- `make` version used: 4.2.1
- `C/C++` versions used: C11/C++20

# Build notes:
- In order to build the application (and other relevant artifacts), you can use the `buildScript.sh` located
on the root folder of the repo, for example: `$~/path/to/dnssniffer/buildScript.sh`
- The executable will be located under: `$~/path/to/dnsSniffer/build/src/dnsSniffer.out`
- One additional artifact is the: `$~/path/to/dnsSniffer/build/src/tests/unitTestsExe.out`

# Deployment environment notes:
- The final executable ran on the development machine. Also, it was built, unit-tested, and ran a naive run
on a Docker based container that its Docker file you can find in the root folder of the repo. Note, however, 
that the application was not really tested on this Docker image.

# Testing notes:
- In order to unit-test the code, the `GTest` library was installed on the development machine.

# How to use it:
- 1) Navigate to the root folder of the repo (it probably will be called dnsSniffer)
- 2) Build the application using the buildScript: `~/dnsSniffer$ ./buildScript`
- 3) OPTIONAL: For verification, you can run the unit tests: `~/dnsSniffer$ ./build/src/tests/unitTestsExe.out`
- 4) Run the actual executable: `~/dnsSniffer$ ./build/src/dnsSniffer.out` (you might need root permissions)
- 5) You will see several logs on the terminal and eventually you should see the following message: `systemMgrDisplayClientMenu - enter a domain name to query or type 'stop' to end:`
- 6) Enter a domain name you wish to query, for example: cnn.com and then press enter. If DNS response packets with the cnn.com were captured you should see output similar
to the following:
cnn.com
systemMgrDisplayClientMenu - you entered: cnn.com
systemMgrDisplayUserDomainInfo - domain name:cnn.com has the following addresses
151.101.3.5 -> 151.101.195.5 -> 151.101.131.5 -> 151.101.67.5 -> NULL
systemMgrDisplayClientMenu - enter a domain name to query or type 'stop' to end:
- 7) Finally, enter `stop` to terminate the program (or close it with Ctrl+C)


# Was is missing:
- Support DNS response with CNAME record type.
- Capturing DNS responses that ride on top of IPv6.

# Some notes/TODOs:
- In case cross compilation is being conducted, then need to set a global debug symbol that will indicate whether the
target machine is little or big endian.

- Before deploying the software, the build script can also run some unit tests (after it builds the software).
However, it will that later the unit test package along with all of its "left overs" will be removed.

- My usage in multi threading was very basic here, not sure that way the pcap_loop and the UI thread are managed
and implemented is optimal.

- There are several places in the code in which I left a "TODO/explanation note". Filter the string "GuyA:" to check them all.
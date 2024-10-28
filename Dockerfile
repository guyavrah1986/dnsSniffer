# Use Ubuntu 22.04 as the base image
FROM ubuntu:22.04

# Set a label for the image
LABEL description="Ubuntu 22.04 with CMake and GCC installed"

# Update the package list and install cmake and gcc
RUN apt-get update && apt-get install -y \
    cmake \
    gcc \
    g++ \
    make \
    libpcap-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy source files of the project to the container
RUN mkdir project
COPY main.c project/main.c
COPY CMakeLists.txt project/CMakeLists.txt
COPY buildScript.sh project/buildScript.sh

# Build the executable
# RUN gcc project/main.c -o dnsSniffer.out
RUN chmod +x project/buildScript.sh

# Run a bash command to build the project
CMD ["bash", "-c", "/project/buildScript.sh"]

# Set the default command to bash
#CMD ["/bin/bash"]
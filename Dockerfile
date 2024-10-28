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
    && rm -rf /var/lib/apt/lists/*

# Copy source files of the project to the container
RUN mkdir project
COPY main.c project/main.c

# Build the executable
RUN gcc project/main.c -o dnsSniffer.out

# Set the default command to bash
CMD ["/bin/bash"]
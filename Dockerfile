# Use Ubuntu 22.04 as the base image
FROM ubuntu:22.04

# Set a label for the image
LABEL description="Ubuntu 22.04 with CMake, GCC, libpcap (and libdbus), GTest and dnsutils (dig) installed"

SHELL ["/bin/bash", "-c"]

# Update the package list and install cmake and gcc
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libpcap-dev \
    libgtest-dev \
    libdbus-1-dev \
    dnsutils \
    && rm -rf /var/lib/apt/lists/*

# Copy the project sources into the container image
RUN mkdir /project
COPY . /project/
RUN ls -l /project/

# Run a bash command to build the project
#CMD ["bash", "-c", "/project/buildScript.sh"]

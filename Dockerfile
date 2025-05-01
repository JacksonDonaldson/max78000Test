FROM ubuntu:24.04

WORKDIR /root

RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    gdb \
    gdb-multiarch \
    gcc-arm-none-eabi \
    binutils-arm-none-eabi \
    make \
    python3.10 \
    git \
    wget \
    libtool \
    texinfo \
    unzip    

# Install MSDK
RUN apt-get install -y unzip && \
    wget https://github.com/analogdevicesinc/msdk/archive/refs/tags/v2024_02.zip && \
    unzip v2024_02.zip && \
    rm -f v2024_02.zip

ENV MAXIM_PATH=/root/msdk-2024_02

WORKDIR /program

ENTRYPOINT ["bash", "-c", "make release && cp build/max78000.elf build/max78000.bin /out"]

# Sample run command:
# docker run --rm -v ./build_out:/out -v ./:/decoder -v ./secrets:/secrets -e DECODER_ID=0xdeadbeef decoder

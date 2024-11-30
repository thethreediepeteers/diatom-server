FROM debian:bookworm-slim

WORKDIR /home

COPY . ./
EXPOSE 3000

ENV CC=clang
ENV CXX=clang++
ENV RELEASE=1

# Update package list and install necessary packages
RUN apt-get update && \
    apt-get install -y clang libc-dev binutils make git g++ zlib1g-dev libssl-dev && \
    rm -rf /var/lib/apt/lists/*

RUN sh install.sh
RUN make

CMD ["./server"]

FROM alpine:3.19@sha256:13b7e62e8df80264dbb747995705a986aa530415763a6c58f84a3ca8af9a5bcd
WORKDIR /home

COPY . ./
EXPOSE 3000

ENV CC=clang
ENV CXX=clang++
ENV RELEASE 1

RUN apk update
RUN apk add clang libc-dev binutils make git
RUN sh install.sh
RUN make

CMD ["./server"]

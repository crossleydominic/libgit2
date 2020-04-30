FROM ubuntu:bionic

RUN apt-get update
RUN apt-get install -y build-essential g++ cmake libssl-dev python3

ADD . /libgit2-src/

RUN mkdir /libgit2-src/build/
RUN mkdir /build-output

WORKDIR /

ENTRYPOINT libgit2-src/docker-build-entrypoint.sh

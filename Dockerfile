FROM ubuntu:18.04 as builder

LABEL Description="A model checker for safety and temporal properties."

# pkg-config, uuid-dev and antlr4

RUN apt-get update && apt-get install --no-install-recommends -y pkg-config uuid-dev antlr4 git python build-essential wget tar cmake libtinfo-dev

# readline setup
RUN wget ftp://ftp.gnu.org/gnu/readline/readline-8.0.tar.gz \
    && tar -xvf readline-8.0.tar.gz \
    && cd readline-8.0 \
    && ./configure \
    && make \
    && make install \
    && make clean


WORKDIR /usr/src/complyer
COPY . .
RUN mkdir build \
    && cd build \
    && cmake .. \
    && make

ENV LD_LIBRARY_PATH "$LD_LIBRARY_PATH:/usr/src/complyer/build"
ENTRYPOINT ["/usr/src/complyer/build/complyer"]

# docker run -it --rm -v `pwd`:/in image /in/file.smv [options] - passing file.smv (present in current directory) as input to complyer 


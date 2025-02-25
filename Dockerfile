FROM docker.io/gcc:14

COPY . /usr/src/lso-server
WORKDIR /usr/src/lso-server
RUN make -j
EXPOSE 9999
CMD ["./main"]
FROM gcc:14

COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN make -j
EXPOSE 9999
CMD ["./main"]
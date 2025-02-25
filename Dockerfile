FROM gcc:14

COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN make -j
EXPOSE 9999
CMD ["./main"]

# Eseguire:
# docker build -t lso-server .
# docker run -it -p 9999:9999 --rm --name running-lso-server lso-server 

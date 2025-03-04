FROM docker.io/alpine:3.21 AS build
COPY . /usr/src/lso-server
WORKDIR /usr/src/lso-server
RUN apk update && \
    apk add --no-cache build-base && \
    make -j


FROM docker.io/alpine:3.21
COPY --from=build /usr/src/lso-server/main /usr/src/lso-server/main
WORKDIR /usr/src/lso-server
EXPOSE 9999
ENTRYPOINT ["./main"]
#!/bin/bash

if [ "$1" == "podman" ]; then
    docker="podman"
elif [ "$1" == "podman-remote" ]; then # for podman on MacOS or WSL
    docker="podman-remote-static-linux_amd64"
else
    docker="docker"
fi

$docker build -t lso-server . && \
$docker run -it --rm -p 9999:9999 --name running-lso-server lso-server

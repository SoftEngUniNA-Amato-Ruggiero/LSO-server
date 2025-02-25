#!/bin/bash

if [ $1 == "podman" ]; then
    docker="podman"
else
    docker="docker"
fi

$docker build -t lso-server .
$docker run -it -p 9999:9999 --name running-lso-server lso-server
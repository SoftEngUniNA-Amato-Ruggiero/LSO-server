#!/bin/bash

docker build -t lso-server .
docker run -it -p 9999:9999 --rm --name running-lso-server lso-server
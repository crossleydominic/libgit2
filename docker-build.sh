#!/bin/bash

DOCKER_BUILD_DIR="docker-build"
DOCKER_IMAGE_NAME="libgit2-docker-build-env"
DOCKER_CONTAINER_NAME=$DOCKER_IMAGE_NAME

if [[ -d $DOCKER_BUILD_DIR ]]; then
  rm -r $DOCKER_BUILD_DIR
fi

mkdir $DOCKER_BUILD_DIR

if [[ -d build ]]; then
  rm -r build
fi

docker build -t $DOCKER_IMAGE_NAME .

docker run -it --rm \
  --name "$DOCKER_CONTAINER_NAME" \
  -v "$(pwd)/$DOCKER_BUILD_DIR":/build-output \
  "$DOCKER_IMAGE_NAME"


FROM ubuntu

ARG DEBIAN_FRONTEND=noninteractive
RUN apt update && apt-get -y install unzip zip clang cmake ninja-build pkg-config libgtk-3-dev xvfb cargo wget git g++
RUN wget https://github.com/premake/premake-core/releases/download/v5.0.0-alpha15/premake-5.0.0-alpha15-linux.tar.gz
RUN tar -xvf premake-5.0.0-alpha15-linux.tar.gz
RUN mv premake5 /usr/bin/

RUN apt-get -y install python
# ADD skia/dependencies/make_dependencies.sh /app/skia/dependencies/make_dependencies.sh
ADD skia/dependencies/make_skia.sh /app/skia/dependencies/make_skia.sh
ADD skia/dependencies/make_glfw.sh /app/skia/dependencies/make_glfw.sh
WORKDIR /app/skia/dependencies
# RUN /app/skia/dependencies/make_dependencies.sh
RUN /app/skia/dependencies/make_skia.sh
RUN /app/skia/dependencies/make_glfw.sh

WORKDIR /app/packages/peon_process
ADD rive /app/rive
ADD skia /app/skia
WORKDIR /app/skia/renderer
# RUN /app/skia/renderer/build.sh

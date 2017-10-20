FROM fedora:25
MAINTAINER David Wagner <david.wagner@easymile.com>

RUN echo "fastestmirror=True" >> /etc/dnf/dnf.conf

RUN dnf install -y \
    gcc-c++ \
    cmake \
    make \
    gstreamer1-rtsp-server-devel \
    gstreamer1-plugins-good \
    boost-program-options \
    boost-devel \
    catch-devel

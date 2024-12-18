FROM debian 
LABEL maintainer="fnugus" 
#####################
# BUILD CODECHECKER #
#####################
RUN apt-get -y update 
RUN apt-get -y install \
    clang \
    clang-tidy \
    cppcheck \
    g++ \
    build-essential \
    curl \
    gcc-multilib \
    git python3-dev \
    python3-venv \
    python3-setuptools \
    libpq-dev
RUN curl -sL https://deb.nodesource.com/setup_16.x | bash -
RUN apt-get install -y nodejs
RUN git clone https://github.com/Ericsson/CodeChecker.git --depth 1 ~/codechecker && \
    cd ~/codechecker && \
    make venv
RUN cd ~/codechecker && \
    . $PWD/venv/bin/activate && \
    make package

#################
# MOUNT PROJECT #
#################
#VOLUME 

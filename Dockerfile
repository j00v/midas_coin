FROM ubuntu:16.04

RUN apt-get update && \
    apt-get --no-install-recommends --yes install \
         git \
         automake \
         build-essential \
         libtool \
         autotools-dev \
         autoconf \
         pkg-config \
         libssl-dev \ 
         libboost-all-dev \
         libevent-dev \
         bsdmainutils \
         vim \
         software-properties-common && \
         rm -rf /var/lib/apt/lists/* && apt-get clean

RUN add-apt-repository ppa:bitcoin/bitcoin && \
    apt-get update && \
    apt-get --no-install-recommends --yes install \
          libdb4.8-dev \
          libdb4.8++-dev \
          libminiupnpc-dev && \
          rm -rf /var/lib/apt/lists/* && apt-get clean

WORKDIR /midas

COPY . .

RUN ./autogen.sh && \
    ./configure && \
    make && \
    strip src/midasd && \
    strip src/midas-cli && \
    mv src/midasd /usr/local/bin/ && \
    mv src/midas-cli /usr/local/bin/ && \
    rm -rf /midas 

VOLUME ["/root/.midas"]

EXPOSE 22331

CMD exec ipsd && tail -f /root/.midas/debug.log

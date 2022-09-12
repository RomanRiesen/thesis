# docker build -t "fprime_with_reelay" .
# docker run -it -p 5000:5000 fprime_with_reelay
FROM ubuntu:latest
ENV LD_LIBRARY_PATH=/usr/local/lib
RUN apt update && apt install -y git python3-pip dh-autoreconf libboost-all-dev cmake &&\
    git clone https://github.com/doganulus/reelay &&\
    git clone https://github.com/nasa/fprime &&\
    git clone https://github.com/ivmai/cudd &&\
    pip install -r fprime/requirements.txt &&\
    cd ./cudd &&\
    autoreconf -f -i &&\
    cd ../reelay &&\
    make install &&\
    make cudd &&\
    make cudd-install

COPY ./components_example/Ref fprime/Ref
COPY ./components_example/config fprime/config
EXPOSE 5000
RUN cd ./fprime/Ref && yes y | fprime-util purge && fprime-util generate && fprime-util build -j 8
CMD cd ./fprime/Ref && fprime-gds --gui-addr 0.0.0.0

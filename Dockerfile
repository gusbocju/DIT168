# docker run --rm -ti -v $PWD:/opt/sources alpine:3.7 /bin/sh
FROM alpine:3.7 AS builder
MAINTAINER Julian Bock gusbocju@student.gu.se
RUN apk update && \
    apk --no-cache add \
        ca-certificates \
        cmake \
        g++ \
        make && \
    apk add libcluon --no-cache --repository https://chrberger.github.io/libcluon/alpine/v3.7 --allow-untrusted
ADD . /opt/sources
WORKDIR /opt/sources
RUN cd /opt/sources && \
    mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make && \
    cd src/V2V/ && \
    cp MARBLE.V2V /tmp && \
    cd ../DS4/ && \
    cp MARBLE.DS4 /tmp && \
    cd ../demo/ds4/ && \
    cp MARBLE.DS4.Demo /tmp && \
    cd ../networking/ && \
    cp MARBLE.Networking.Demo.Sender /tmp && \
    cp MARBLE.Networking.Demo.Receiver /tmp && \
    cd ../safety-check/ && \
    cp MARBLE.SafetyCheck.Main /tmp && \
    cp MARBLE.SafetyCheck.RemoteControl /tmp

# Deploy.
FROM alpine:3.7
MAINTAINER Julian Bock gusbocju@student.gu.se
RUN apk update && \
    apk add libcluon --no-cache --repository https://chrberger.github.io/libcluon/alpine/v3.7 --allow-untrusted && \
    mkdir /opt
WORKDIR /opt
COPY --from=builder /tmp/MARBLE.V2V .
COPY --from=builder /tmp/MARBLE.DS4 .
COPY --from=builder /tmp/MARBLE.DS4.Demo .
COPY --from=builder /tmp/MARBLE.Networking.Demo.Sender .
COPY --from=builder /tmp/MARBLE.Networking.Demo.Receiver .
COPY --from=builder /tmp/MARBLE.SafetyCheck.Main .
COPY --from=builder /tmp/MARBLE.SafetyCheck.RemoteControl .
CMD ["/bin/sh"]
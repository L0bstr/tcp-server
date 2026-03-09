FROM alpine:latest
RUN apk add --no-cache musl-dev gcc make cmake

WORKDIR /server/

COPY src/ ./src/
COPY include/ ./include/
COPY ./CMakeLists.txt ./

RUN cmake -B build && cmake --build build

EXPOSE 3000

CMD ["./build/tcp-server", "--ip", "0.0.0.0", "--port", "3000"]

FROM alpine:latest
RUN apk add --no-cache gcc musl-dev make

WORKDIR /server/

COPY src/ ./src/
COPY include/ ./include/
COPY ./Makefile ./

RUN make

EXPOSE 3000

CMD ["./build/server", "--port", "3000"]

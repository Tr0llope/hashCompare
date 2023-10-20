FROM gcc:latest

COPY . /app

WORKDIR /app

RUN apt-get update && apt-get -y install libssl-dev && apt-get -y install valgrind && make




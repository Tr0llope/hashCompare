FROM gcc:latest

COPY . /app

WORKDIR /app

RUN make

CMD ["cat rockyou.txt | ./hash_compare -G rainbow_table.txt","cat hashes_input.txt | ./hash_compare -L rainbow_table.txt"]




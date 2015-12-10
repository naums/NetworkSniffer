CC=g++
LIB=-lpthread -lz -lrt -lrdkafka++ -lrdkafka

all:
	$(CC) -g -Wall -pedantic -o "test" test.cpp kproducer.cpp kconsumer.cpp $(LIB)
	$(CC) -g -Wall -pedantic -o "cons" cons.cpp kproducer.cpp kconsumer.cpp $(LIB)
	

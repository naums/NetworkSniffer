include ../../Makefile.main

BUILD=../../$(_BUILD)

$(BUILD)thread.o: thread.cpp
	g++ thread.cpp -c -o $(BUILD)thread.o

test: $(BUILD)thread.o
	g++ $(BUILD)thread.o main.cpp -lpthread -o $(BUILD)thread.exec

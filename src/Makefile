CC=g++
LIB=-lpthread -lpcap -lmysqlpp
CFLAGS=-Wall -pedantic -g
PRE=pcap/pcap.o pcap/trace.o base/thread/thread.o base/debug/debug.o base/date/date.o config.o sniffer.o mysql/mysql.o mysql/store.o compute/compute.o compute/process.o compute/dep.o base/structures/nodeIdentificator.o 

all: $(PRE) sniff.o compute.o
	$(CC) $(CFLAGS) $(PRE) sniff.o $(LIB) -o "sniff.exec"
	$(CC) $(CFLAGS) $(PRE) compute.o $(LIB) -o "compute.exec"


.cpp.o:
	$(CC) $(CFLAGS) -c -o $@ $<

doc:
	doxygen doc_template

clean:
	rm $(PRE) sniff.o compute.o sniff.exec compute.exec

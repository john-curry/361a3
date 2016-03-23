CC=g++
FLAGS=-Wall -pedantic -g -std=c++11
LINKS=-lpcap
OUT=sniff
ARGS=ip-trace-1.pcap #trace-icmp.pcap#linux.pcap #ip-trace-1.pcap #trace-icmp.pcap linux.pcap
FILES=$(wildcard *.cpp)
OBJS=$(FILES:.cpp=.o)
HEADERS=$(wildcard *.h)

$(OUT): $(OBJS)
	$(CC) $(FLAGS) $^ -o $@ $(LINKS)


%.o: %.cpp $(HEADERS)
	$(CC) $(FLAGS) -c $< 

%.h:

clean:
	rm $(OUT) *.o

run: $(OUT)
	./$(OUT) $(ARGS)

debug:
	gdb --args $(OUT) $(ARGS)

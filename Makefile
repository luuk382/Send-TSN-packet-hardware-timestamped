files := main.o Transmitter/transmitter.o Nodes/general_node.o
GXX := g++
CC := g++ # because otherwise the linker is for c rather than c++

main: ${files}

clean:
	rm -f ${files}
	rm -f main

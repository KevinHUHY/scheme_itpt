CC = clang++
SRCS    = $(shell /bin/ls *.cc)
CFLAGS   = -DOP_ASSIGN

.SUFFIXES: $(SUFFIXES) .cpp

%.o: %.cpp
	g++ -c $(CFLAGS) -fno-elide-constructors $<

OBJS = main.o parse.o eval_helper.o eval.o Cell.o IntCell.o DoubleCell.o SymbolCell.o ConsCell.o ProcedureCell.o

main: $(OBJS)
	$(CC) -g -Wall $(CFLAGS) -o $@ $(OBJS) -lm

main.o: Cell.hpp cons.hpp parse.hpp eval.hpp main.cpp
	$(CC) -c -g -Wall -std=c++11 main.cpp

parse.o: Cell.hpp cons.hpp parse.hpp parse.cpp
	$(CC) -c -g -Wall -std=c++11 parse.cpp

eval_helper.o: arith.hpp Cell.hpp cons.hpp eval.hpp eval_helper.hpp \
							eval_helper.cpp bstmap.hpp
	$(CC) -c -g -Wall -std=c++11 eval_helper.cpp

eval.o: Cell.hpp cons.hpp eval.hpp eval.cpp bstmap.hpp
	$(CC) -c -g -Wall -std=c++11 eval.cpp

Cell.o: Cell.hpp Cell.cpp
	$(CC) -c -g -Wall -std=c++11 Cell.cpp

IntCell.o: Cell.hpp IntCell.hpp IntCell.cpp
	$(CC) -c -g -Wall -std=c++11 IntCell.cpp

DoubleCell.o: Cell.hpp DoubleCell.hpp DoubleCell.cpp
	$(CC) -c -g -Wall -std=c++11 DoubleCell.cpp

SymbolCell.o: Cell.hpp eval_helper.hpp SymbolCell.hpp SymbolCell.cpp
	$(CC) -c -g -Wall -std=c++11 SymbolCell.cpp

ConsCell.o: Cell.hpp Conscell.hpp ConsCell.cpp
	$(CC) -c -g -Wall -std=c++11 ConsCell.cpp

ProcedureCell.o: Cell.hpp eval_helper.hpp ProcedureCell.hpp ProcedureCell.cpp
	$(CC) -c -g -Wall -std=c++11 ProcedureCell.cpp

clean:
	rm -f core *~ $(OBJS) main main.exe testoutput.txt

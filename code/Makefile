CXX=g++
LINK.o=g++
CXXFLAGS=-Wall -std=c++0x
LOADLIBES= -static
OBJFILES =  FiniteStateMachine.o\
	    State.o\
	    TokenList.o\
	    Token.o\
	    Job.o\
	    Process.o\
	    Shell.o\
	    System.o\
	    TokenList2Job.o\
	    Behavior.o\
	    Utility.o

.PHONY: debug
debug: CXXFLAGS+=-DDEBUG
debug: Phase1 Phase1Ext Phase2

.PHONY: release
release: Phase1 Phase1Ext Phase2


Phase1: Phase1.o $(OBJFILES)
Phase1Ext: Phase1Ext.o $(OBJFILES)
Phase2: Phase2.o $(OBJFILES)

.PHONY: clean
clean:
	rm $(OBJFILES) Phase1.o Phase1Ext.o Phase2.o

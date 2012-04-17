#----------------------------------------------------
# makefile
#----------------------------------------------------
FLAGS= -g -Wall
run: ctree.o tree.o main.o
	g++ $(FLAGS) -o run tree.o ctree.o main.o -lboost_thread
tree.o: tree.cpp tree.h
	g++ $(FLAGS) -c tree.cpp
ctree.o: ctree.cpp ctree.h tree.cpp tree.h
	g++ $(FLAGS) -c ctree.cpp
main.o: main.cpp tree.h ctree.h
	g++ $(FLAGS) -c main.cpp
clean:
	rm -f *.o *~ run




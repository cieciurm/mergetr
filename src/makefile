CC = cc
CFLAGS = -Wall 
OBJS = mergetr.o mergetrIO.o parser.o addHoles.o triangulation.o glueMesh.o

mtr: $(OBJS) triangle/triangle.o
	$(CC) $(CFLAGS) $(OBJS) triangle/triangle.o -lm -o $@ 

triangle/triangle.o: 
	cd triangle; make trilibrary

# Testing tasks

test1: mtr 
	./mtr -p test/O1 -a0.01 test/A.1 test/F.1

test2: mtr 
	./mtr -p test/O2 -a.01 test/D.1 test/F.1

test3: mtr 
	./mtr -p test/O1 test/B.1 test/F.1 -q30 -a0.01 

test4: mtr 
	./mtr -p test/O3 -a.7 test/A.1 test/D.1

# Showme task

showme: triangle/showme test/result.node
	./triangle/showme test/result

triangle/showme:
	cd triangle; make showme

# Clean up task

clean: 
	rm $(OBJS)

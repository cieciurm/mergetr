CC = cc
CFLAGS = -Wall 
OBJS = mergetr.o mergetrIO.o parser.o addHoles.o triangulation.o glueMesh.o

mergetr: $(OBJS) triangle/triangle.o
	$(CC) $(CFLAGS) $(OBJS) triangle/triangle.o -lm -o $@ 

triangle/triangle.o: 
	cd triangle; make trilibrary

# Testing tasks

test1: mergetr 
	./mergetr -p test/O1 -a0.01 test/A.1 test/F.1

test2: mergetr 
	./mergetr -p test/O2 -a.01 test/D.1 test/F.1

test3: mergetr 
	./mergetr -p test/O3 -a.7 test/A.1 test/D.1

test4: mergetr 
	./mergetr -p test/O1 test/B.1 test/F.1 -q30 -a0.01 


# Showme task

showme: triangle/showme test/result.node
	./triangle/showme test/result

triangle/showme:
	cd triangle; make showme

# Clean up task

clean: 
	rm $(OBJS)

TARGET=excercise2
CC=gcc
OBJS=linkedList.o main.o scoreBoard.o mainFunc.o fileFunc.o studentData.o

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

fileFunc.o mainFunc.o: fileFunc.h
studentData.o mainFunc.o studentData.o: studentData.h
linkedList.o fileFunc.o mainFunc.o: linkedList.h
scoreBoard.o mainFunc.o: scoreBoard.h
main.o: mainFunc.h

clean:
	rm -f $(TARGET) $(OBJS)
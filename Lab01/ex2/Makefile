TARGET=excercise2
CC=gcc
OBJS=linkedList.o main.o scoreBoard.o mainFunc.o fileFunc.o student.o

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

fileFunc.o mainFunc.o: fileFunc.h
student.o mainFunc.o student.o: student.h
linkedList.o fileFunc.o mainFunc.o: linkedList.h
scoreBoard.o mainFunc.o: scoreBoard.h
student.o mainFunc.o main.o fileFunc.o: mainFunc.h

clean:
	rm -f $(TARGET) $(OBJS)
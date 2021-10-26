TARGET=exercise2
CC=gcc
OBJS=linkedList.o main.o scoreBoard.o mainFunc.o fileFunc.o student.o

$(TARGET): $(OBJS)
	@echo "Linking ..."
	@$(CC) -o $@ $(OBJS)
	@echo "Compilation finished."

fileFunc.o mainFunc.o: fileFunc.h
student.o mainFunc.o student.o: student.h
linkedList.o fileFunc.o mainFunc.o: linkedList.h
scoreBoard.o mainFunc.o: scoreBoard.h
student.o mainFunc.o main.o fileFunc.o: mainFunc.h

clean:
	@echo "Cleaning ..."
	@rm -f $(TARGET) $(OBJS)
	@echo "Cleaned."
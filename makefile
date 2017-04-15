CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES =parser.o lexer.o driver.o SymbolTable.o AST.o
TARGET = stage1exe
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS) -lm -g
clean:
	rm -f $(OBJFILES) $(TARGET)

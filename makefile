CC = gcc
CFLAGS = -Wall
LDFLAGS = -g -lm
OBJFILES =parser.o lexer.o driver.o SymbolTable.o AST.o codegen.o
TARGET = toycompiler
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET)

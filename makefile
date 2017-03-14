CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES =parser.o lexer.o driver.o
TARGET = stage1exe
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET)

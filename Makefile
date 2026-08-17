CC = gcc
CFLAGS = -Wall -g
TARGET = healwish
SOURCES = main.c admin.c doctor.c patient.c md5.c utils.c

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: clean

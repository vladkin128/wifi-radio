# Makefile for Wifi-radio

TARGET = control

CC = gcc
#CFLAGS = -Wall 
#CFLAGS += -g
#CFLAGS += -O3
CFLAGS += -D__DEBUG__

OBJ = $(TARGET).o    \
	mpc.o \
	display.o \
	main.o \
	term.o \
	uart.o

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ $(LDFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(LDFLAGS)

clean:
	rm -f *.o *.su $(TARGET) 

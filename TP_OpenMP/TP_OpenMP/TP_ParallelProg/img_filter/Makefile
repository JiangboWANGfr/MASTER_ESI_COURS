CC=gcc
DEBUG=-g
OPENMP=-fopenmp
CFLAGS=$(DEBUG) -Wall -Wextra -pedantic -Wimplicit-fallthrough=0 -O3 -std=c11 $(OPENMP)
LDFLAGS=

TARGET=convolution_filter

all: $(TARGET)

$(TARGET): main.o img_convolution_seq.o img_mgmt.o
	$(CC) $(CFLAGS) -o $@ $^

%.o:%.c utils.h img_mgmt.h img_convolution.h
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o

mrproper: clean
	rm -f $(TARGET)

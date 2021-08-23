#Directory Macros
IDIR = ./include
ODIR = ./obj
SDIR = ./src
BUILDDIR = ./build

#compiler macros
CC = gcc
CFLAGS = -I $(IDIR) -Wall
LIBS = -lpthread
HFILES = $(IDIR)/*.h
OFILES = $(ODIR)/main.o $(ODIR)/ADC.o $(ODIR)/EndianSwap.o $(ODIR)/Errors.o $(ODIR)/UDP-Server.o

$(BUILDDIR)/bb-osclliscope.out: $(OFILES) 
	$(CC) -o $@ $(OFILES) $(LIBS)

$(ODIR)/main.o: $(SDIR)/main.c $(HFILES)
	$(CC) -c $(SDIR)/main.c $(CFLAGS) -o $@

$(ODIR)/ADC.o: $(SDIR)/ADC.c $(HFILES)
	$(CC) -c $(SDIR)/ADC.c $(CFLAGS) -o $@

$(ODIR)/EndianSwap.o: $(SDIR)/EndianSwap.c $(HFILES)
	$(CC) -c $(SDIR)/EndianSwap.c $(CFLAGS) -o $@

$(ODIR)/Errors.o: $(SDIR)/Errors.c $(HFILES)
	$(CC) -c $(SDIR)/Errors.c $(CFLAGS) -o $@

$(ODIR)/UDP-Server.o: $(SDIR)/UDP-Server.c $(HFILES)
	$(CC) -c $(SDIR)/UDP-Server.c $(CFLAGS) -o $@

clean:
	-rm -f $(OFILES) $(BUILDDIR)/bb-osclliscope.out

rebuild: clean $(BUILDDIR)/bb-osclliscope.out
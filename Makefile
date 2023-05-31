COMPILER ?= mpicc
CFLAGS = -O2 -Wall -Wno-variadic-macros -pedantic#-DDEBUG -DDEBUG_QUICKSORT_DETAIL
LDFLAGS = -g 
LDLIBS =

SRCDIR = C/src
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

EXECUTABLE = quicksort

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(COMPILER) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(COMPILER) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJDIR)/*.o $(EXECUTABLE)

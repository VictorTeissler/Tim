TARGET = tim
LINKER   = g++
CPPFLAGS=`pkg-config libvmi --cflags` `pkg-config glib-2.0 --cflags`
CPPFLAGS+=`pkg-config --cflags json-glib-1.0` -I./include  -I/usr/include/python3.6m
CXX=g++
RM=rm -f
LDLIBS=`pkg-config libvmi --libs` `pkg-config gobject-2.0 --libs` `pkg-config --libs json-glib-1.0`

INCDIR   = includes
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SRCS=$(wildcard $(SRCDIR)/*.cc)
OBJECTS  := $(SRCS:$(SRCDIR)/%.cc=$(OBJDIR)/%.o)
INCLUDES := $(wildcard $(INCDIR)/*.h)

$(BINDIR)/$(TARGET): $(OBJECTS)
	    @$(LINKER) $(OBJECTS) $(LDLIBS) -o $@
		    @echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	    @$(CC) $(CPPFLAGS) -c $< -o $@
		    @echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	$(RM) $(OBJECTS)
	$(RM) $(BINDIR)/tim
	@echo "Cleanup complete!"

so:
	@$(RM) $(BINDIR)/tim.o
	@$(RM) $(BINDIR)/rekall_parser.o
	@$(RM) $(BINDIR)/proc.o
	@$(RM) $(BINDIR)/ctx.o
	@$(RM) $(BINDIR)/tim.so
	@$(CXX) $(LDLIBS) -c -fPIC -o $(BINDIR)/rekall_parser.o \
		$(SRCDIR)/rekall_parser.cc $(CPPFLAGS)
	@$(CXX) $(LDLIBS) -c -fPIC -o $(BINDIR)/tim.o $(SRCDIR)/main.cc $(CPPFLAGS)
	@$(CXX) $(LDLIBS) -c -fPIC -o $(BINDIR)/proc.o $(SRCDIR)/proc.cc $(CPPFLAGS)
	@$(CXX) $(LDLIBS) -c -fPIC -o $(BINDIR)/ctx.o $(SRCDIR)/ctx.cc $(CPPFLAGS)
	@$(CXX) $(LDLIBS) -shared -Wl,-soname,tim.so -o $(BINDIR)/tim.so $(BINDIR)/tim.o $(BINDIR)/rekall_parser.o $(BINDIR)/proc.o $(BINDIR)/ctx.o

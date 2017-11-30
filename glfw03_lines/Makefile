
CXX=g++ `pkg-config --cflags glfw3 gl` -std=c++11 -Wall
LD=g++
LDFLAGS=`pkg-config --libs glfw3 gl`

SRCS=$(wildcard *.cpp)
OBJ=$(subst .cpp,.o,$(SRCS))

#see http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

%.o : %.c
%.o : %.c $(DEPDIR)/%.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

all : lines

clean :
	rm -f lines *.o

lines : ${OBJ}
	${LD} ${OBJ} $(LDFLAGS) -o lines

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))

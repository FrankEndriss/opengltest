
CXX=g++ `pkg-config --cflags glfw3 gl` -std=c++11 -Wall
LD=g++
LDFLAGS=`pkg-config --libs glfw3 gl`

OBJDIR := obj
SRCS=$(wildcard *.cpp)
#next line makes "*.cpp" -> "obj/*.o"
OBJ=$(addprefix $(OBJDIR)/,$(subst .cpp,.o,$(SRCS)))

#see http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
#and https://www.gnu.org/software/make/manual/html_node/index.html

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
$(shell mkdir -p $(OBJDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

$(OBJDIR)/%.o : %.c
$(OBJDIR)/%.o : %.c $(DEPDIR)/%.d
	$(COMPILE.c) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(OBJDIR)/%.o : %.cpp
$(OBJDIR)/%.o : %.cpp $(DEPDIR)/%.d
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

all : lines

clean :
	rm -rf lines obj

lines : ${OBJ}
	${LD} ${OBJ} $(LDFLAGS) -o lines

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))

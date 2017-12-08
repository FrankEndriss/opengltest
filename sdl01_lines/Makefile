
#configure packages glfw3 and gl for openGL-stuff
#-std=c++11 beause thats the HOT stuff
#-Wall because we are pedantic
#-DBOOST_LOG_DYn_LINK for logging with boost-log library
CXX=g++ `pkg-config --cflags glu sdl2 gl` -std=c++11 -Wall -DBOOST_LOG_DYN_LINK
LD=g++
#-lboost_log and -lpthread for logging wicht boost-log library
LDFLAGS=`pkg-config --libs glu sdl2 gl` -lboost_log  -lpthread

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

TARGET = balld
CRONSH = balld_cron.sh
OBJ_PATH = objs
PREFIX_BIN = /usr/bin

CC = g++
INCLUDES = -I /usr/local/include/ -I .
LIBS = -lpthread -ljson -lpcre++ -lpcre -L /usr/local/lib -lcurl
CFLAGS = -Wall -O2 -Werror
LINKFLAGS =

SRCDIR = .

#C_SOURCES = $(wildcard *.c)
C_SRCDIR = $(SRCDIR)
C_SOURCES = $(foreach d,$(C_SRCDIR),$(wildcard $(d)/*.c) )
C_OBJS = $(patsubst %.c, $(OBJ_PATH)/%.o, $(C_SOURCES))

#CPP_SOURCES = $(wildcard *.cpp)
CPP_SRCDIR = $(SRCDIR)
CPP_SOURCES = $(foreach d,$(CPP_SRCDIR),$(wildcard $(d)/*.cpp) )
CPP_OBJS = $(patsubst %.cpp, $(OBJ_PATH)/%.o, $(CPP_SOURCES))

default:init compile

$(C_OBJS):$(OBJ_PATH)/%.o:%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(CPP_OBJS):$(OBJ_PATH)/%.o:%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

init:
	$(foreach d,$(SRCDIR), mkdir -p $(OBJ_PATH)/$(d);)

test:
	@echo "C_SOURCES: $(C_SOURCES)"
	@echo "C_OBJS: $(C_OBJS)"
	@echo "CPP_SOURCES: $(CPP_SOURCES)"
	@echo "CPP_OBJS: $(CPP_OBJS)"

compile:$(C_OBJS) $(CPP_OBJS)
	$(CC)  $^ -o $(TARGET)  $(LINKFLAGS) $(LIBS)

clean:
	rm -rf $(OBJ_PATH)
	rm -f $(TARGET)

install: $(TARGET)
	cp -a $(TARGET) $(PREFIX_BIN)
	mkdir -p /etc/webhelper.d/
	cp -a $(CRONSH) /etc/webhelper.d/

uninstall:
	rm -f $(PREFIX_BIN)/$(TARGET)

rebuild: clean init compile

TAR_DIR=balld-v$(VERSION)-$(shell date +"%Y-%m-%d")
dist:
	mkdir $(TAR_DIR)
	cp -a json *.cpp *.h *.cbp *.depend *.layout Makefile balld_cron.sh Readme.txt $(TAR_DIR)
	tar jcf $(TAR_DIR).tar.bz2 $(TAR_DIR)
	rm -rf $(TAR_DIR)

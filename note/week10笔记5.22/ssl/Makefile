#common makefile header

DIR_INC = ./include
DIR_BIN = ./bin
DIR_LIB = ./libs
# TARGET 功能编译后可执行文件叫什么名字
TARGET	= server
BIN_TARGET = $(DIR_BIN)/$(TARGET)

CROSS_COMPILE = 
CFLAGS = -g -Wall -I$(DIR_INC)

ifdef LINUX64
LDFLAGS := -L$(DIR_LIB)/x64
else
LDFLAGS := -L$(DIR_LIB)/x86 
endif
LDFLAGS += -lmsc -lrt -ldl -lpthread -lstdc++ -lssl -lcrypto

# c文件未来统一存放到 res 目录下
OBJECTS := $(patsubst %.c,%.o,$(wildcard res/*.c))

$(BIN_TARGET) : $(OBJECTS)
	$(CROSS_COMPILE)gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o : %.c
	$(CROSS_COMPILE)gcc -c $(CFLAGS) $< -o $@
clean:
	@rm -f *.o $(BIN_TARGET)

.PHONY:clean

#common makefile foot

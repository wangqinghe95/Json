# vpath %h ./include ../FileSystem/include ../Logger/include
OBJOUT := ./out
EXEOUT := ./out/
# INCLUDE_DIR := ./include -I/home/user/Desktop/Github/FilesSystem/include -I../Logger/include
INCLUDE_DIR := ./include -I../FilesSystem/include -I../Logger/include
SRC_DIR_TEST = ./unittest
SRC_DIR_JSON := ./src
SRC_DIR_FILES := ../FilesSystem/src
SRC_DIR_LOGGER := ../Logger/src
LIB := -lpthread

SRC := $(wildcard $(SRC_DIR_TEST)/*.cpp)
SRC += $(wildcard $(SRC_DIR_JSON)/*.cpp)
SRC += $(wildcard $(SRC_DIR_FILES)/*.cpp)
SRC += $(wildcard $(SRC_DIR_LOGGER)/*.cpp)
OBJ := $(patsubst %.cpp,%.o, $(SRC))

TARGET := testJson
CC := g++
CXXFLAG = -std=c++11 -g
CFLAGS := -I$(INCLUDE_DIR)

all: PRE_CHECK $(TARGET) 
$(TARGET) : $(OBJ)
	$(CC) $(CFLAGS) $(CXXFLAG) $^ -o $(TARGET) $(LIB)
	mv $^ $(OBJOUT)
	@echo "Compiling" $@ "end"

%.o : %.cpp
	echo "Compiling " $< "...."
	$(CC) $(CFLAGS) $(CXXFLAG) -o $@ $(LIB) -c $^
	@echo "Compile" $@ "end"

PRE_CHECK:
	@if [ -f $(TARGET) ];then	\
		echo "make clean";	\
		make clean;		\
	fi
	@if [ ! -d $(OBJOUT) ];then \
		echo "Folder $(OBJOUT) does not exist, creating it..."; \
		mkdir $(OBJOUT); \
	else	\
		echo "Folder already exist";	\
	fi

.PHONY: clean

clean:
	rm -rf $(TARGET) $(OBJOUT)/* $(OBJOUT)
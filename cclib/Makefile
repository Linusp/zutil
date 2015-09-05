all:
define BUILD_LIBRARY
$(if $(wildcard $@),@rm $@)
$(if $(wildcard ar.mac),@rm ar.mac)
$(if $(filter lib%.a, $^),
@echo CREATE $@ > ar.mac
@echo SAVE >> ar.mac
@echo END >> ar.mac
@ar -M < ar.mac
)
$(if $(filter %.o,$^),@ar -q $@ $(filter %.o, $^))
$(if $(filter %.a, $^),
@echo OPEN $@ > ar.mac
$(foreach LIB, $(filter %.a, $^),
@echo ADDLIB $(LIB) >> ar.mac
)
@echo SAVE >> ar.mac
@echo END >> ar.mac
@ar -M < ar.mac
@rm ar.mac
)
endef

CXX=g++
CC=gcc
CXXFLAGS=-Wall -I $(DIR_INC) -std=gnu++11

# options for debug
DEBUG=YES
ifeq ($(DEBUG), YES)
    CXXFLAGS += -g
endif

MKDIR_P = mkdir -p
DIR_SRC=./src
DIR_TEST=./test
DIR_INC=./inc
DIR_OBJ=./obj
DIR_LIB=./lib
DIR_BIN=./bin


SRC=$(wildcard $(DIR_SRC)/*.cpp)
OBJS=$(patsubst $(DIR_SRC)/%.cpp, $(DIR_OBJ)/%.o, $(SRC))
LIB=$(DIR_LIB)/libzutil.a
TEST_SRC=$(wildcard $(DIR_TEST)/*.cpp)
TEST_BIN=$(patsubst $(DIR_TEST)/%.cpp, $(DIR_BIN)/%, $(TEST_SRC))


all: $(LIB) $(TEST_BIN)

$(LIB): $(OBJS)
	${MKDIR_P} $(DIR_LIB)
	$(BUILD_LIBRARY)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp
	${MKDIR_P} $(DIR_OBJ)
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(DIR_BIN)/%: $(DIR_TEST)/%.cpp
	${MKDIR_P} ${DIR_BIN}
	$(CXX) $(CXXFLAGS) -L $(DIR_LIB) $< -o $@ -lzutil

.PHONY: clean
clean:
	rm $(DIR_BIN)/* -f
	rm $(DIR_OBJ)/*.o -f
	rm $(DIR_LIB)/*.a -f


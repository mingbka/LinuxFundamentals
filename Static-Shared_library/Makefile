CUR_DIR := .
BIN_DIR := $(CUR_DIR)/bin
INC_DIR := $(CUR_DIR)/inc
OBJ_DIR := $(CUR_DIR)/obj
SRC_DIR := $(CUR_DIR)/src
LIB_DIR := $(CUR_DIR)/lib
LIB_DIR_STATIC := $(LIB_DIR)/static
LIB_DIR_SHARED := $(LIB_DIR)/shared

C_FLAG = -I $(INC_DIR)

create_obj_static: 
	gcc -c $(SRC_DIR)/helloWorld.c -o $(OBJ_DIR)/helloWorld.o -I $(INC_DIR)
	gcc -c $(SRC_DIR)/xinChao.c -o $(OBJ_DIR)/xinChao.o -I $(INC_DIR)
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I $(INC_DIR)

create_obj_shared:
	gcc -c -fPIC $(SRC_DIR)/helloWorld.c -o $(OBJ_DIR)/helloWorld.o -I $(INC_DIR)
	gcc -c -fPIC $(SRC_DIR)/xinChao.c -o $(OBJ_DIR)/xinChao.o -I $(INC_DIR)
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I $(INC_DIR)

create_lib_static:
	ar rcs $(LIB_DIR_STATIC)/libhello.a $(OBJ_DIR)/helloWorld.o $(OBJ_DIR)/xinChao.o

create_lib_shared:
	gcc -shared $(OBJ_DIR)/helloWorld.o $(OBJ_DIR)/xinChao.o -o $(LIB_DIR_SHARED)/libhello.so

static: create_obj_static create_lib_static
	gcc  $(OBJ_DIR)/main.o -L$(LIB_DIR_STATIC) -lhello -o $(BIN_DIR)/statically-linked

share: create_obj_shared create_lib_shared
	gcc  $(OBJ_DIR)/main.o -L$(LIB_DIR_SHARED) -lhello -o $(BIN_DIR)/use-shared-library

clean:
	rm -rf $(OBJ_DIR)/*.o $(LIB_DIR_SHARED)/*.so $(LIB_DIR_STATIC)/*.a $(BIN_DIR)/statically-linked $(BIN_DIR)/use-shared-library

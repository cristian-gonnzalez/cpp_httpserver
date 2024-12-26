CC=g++
APP=app
SRC=../dev/app/*cpp \
    ../dev/app/common/*cpp \
    ../dev/app/common/loggers/*cpp \
    ../dev/app/server/*cpp \
    ../dev/app/http/*cpp \
    ../dev/app/http/prot/*cpp
HDR=-I../dev/app \
    -I../dev/app/common \
    -I../dev/app/common/loggers \
    -I../dev/app/server \
    -I../dev/app/http \
    -I../dev/app/http/prot

#
# Compiler flags:
#   -std=c++2a: C++ standard 20 or later
#   -Weffc++: Effective c++ flag that gives tips or improvements in the code
#   -Wall: All warning flag to check casting, types comparision, unused variables, ... 
#   -lpthread: Linking with pthread library that is required to include the thread c++ library
all:
	$(CC) -std=c++2a -g -Wall -Weffc++ $(SRC) $(HDR) -o ../target/$(APP) -lpthread
	chmod 755 ../target/$(APP)


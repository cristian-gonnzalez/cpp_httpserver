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


all:
	$(CC) -std=c++2a -g -Weffc++ $(SRC) $(HDR) -o ../target/$(APP) -lpthread
	chmod 755 ../target/$(APP)

#clean:
#	rm -rf $(SRC)/*.o


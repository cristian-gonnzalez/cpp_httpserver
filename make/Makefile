CC=g++
APP=app
SRC=../dev/app/*cpp \
    ../dev/app/http/*cpp \
    ../dev/app/http/prot/*cpp
HDR=-I../dev/app \
    -I../dev/app/http \
    -I../dev/app/http/prot


all:
	$(CC) -std=c++2a -g $(SRC) $(HDR) -o ../target/$(APP) -lpthread
	chmod 755 ../target/$(APP)

clean:
	rm -rf $(SRC)/*.o


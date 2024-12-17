CC=g++
APP=app
SRC=../dev/app
HDR=../dev/app


all:
	$(CC) -std=c++2a -g $(SRC)/*.cpp -I$(HDR)/*.h -o ../target/$(APP) -lpthread
	chmod 755 ../target/$(APP)

clean:
	rm -rf $(SRC)/*.o


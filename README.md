# HTTP server in modern c++

The idea of the project is to practice making an http server with the lastest c++ standards (c++ 20 or later).

## Steps:

    1. Clone the repository
    2. Run make
      
      ubuntu@Ubuntu:~/make$ make clean
      rm -rf ../dev/app/*.o
      ubuntu@Ubuntu:~/make$ make
      g++ -std=c++2a -g ../dev/app/*.cpp -I../dev/app/*.h -o ../target/app -lpthread
      chmod 755 ../target/app

    3. Run server

      ubuntu@Ubuntu:~/make$ ../target/app 
      Running server

    4. Open a new terminal and run:
      
      ubuntu@Ubuntu:~/test$ python server_test.py 
      hello client
      hello client

    5. Stop the server with CTRL+C

      ubuntu@Ubuntu:~/$ ../target/app 
      Running server
      worker 130139207763648
      ...
      worker 130139197277888
      Hello server
      130139197277888: waiting 5 sec
      Calliing ~Session
      130139086128832: sent message
      Calliing ~Session
      ^CSignal 2 recived
      Stop server
      Stopping server
      Calling ~Server

    

      

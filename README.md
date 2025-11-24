# HTTP server in modern c++

The idea of the project is to practice making an http server with the lastest c++ standards (c++ 20 or later).

## Steps:

    1. Clone the repository
    2. Run make
      
      ubuntu@Ubuntu:~/make$ make
      g++ -c -std=c++2a -g -Wall -Weffc++  ../dev/app/*cpp ../dev/app/common/*cpp ../dev/app/common/loggers/*cpp ../dev/app/server/*cpp ../dev/app/http/*cpp ../dev/app/http/prot/*cpp  -I../dev/app -I../dev/app/common -I../dev/app/common/loggers -I../dev/app/server -I../dev/app/http -I../dev/app/http/prot -I../dev/app/server/cmds/
      g++ *.o -o ../target/app -lpthread

    3. Run server

      ubuntu@Ubuntu:~/make$ ../target/app 
        [ debug ] ../dev/app/server/server.cpp:39:5 [Server::Server(int, size_t)] [2025-11-24 20:41:14] (139942148851520): Calling Server
        [ debug ] ../dev/app/server/server.cpp:222:5 [void Server::run()] [2025-11-24 20:41:14] (139942148847296): Running server
        [ debug ] ../dev/app/server/server.cpp:191:45 [Server::start_tp(size_t)::<lambda()>] [2025-11-24 20:41:14] (139942140454592): tp - waiting ...
        [ debug ] ../dev/app/server/server.cpp:191:45 [Server::start_tp(size_t)::<lambda()>] [2025-11-24 20:41:14] (139942132061888): tp - waiting ...
        [ debug ] ../dev/app/server/server.cpp:191:45 [Server::start_tp(size_t)::<lambda()>] [2025-11-24 20:41:14] (139942123669184): tp - waiting ...
        [ debug ] ../dev/app/server/server.cpp:191:45 [Server::start_tp(size_t)::<lambda()>] [2025-11-24 20:41:14] (139941912770240): tp - waiting ...
        [ debug ] ../dev/app/server/server.cpp:191:45 [Server::start_tp(size_t)::<lambda()>] [2025-11-24 20:41:14] (139941904377536): tp - waiting ...
        [ debug ] ../dev/app/server/server.cpp:191:45 [Server::start_tp(size_t)::<lambda()>] [2025-11-24 20:41:14] (139941895984832): tp - waiting ...
        [ debug ] ../dev/app/server/server.cpp:191:45 [Server::start_tp(size_t)::<lambda()>] [2025-11-24 20:41:14] (139941887592128): tp - waiting ...
        [ debug ] ../dev/app/server/server.cpp:191:45 [Server::start_tp(size_t)::<lambda()>] [2025-11-24 20:41:14] (139941879199424): tp - waiting ...

    4. Open a new terminal and run:
      
      ubuntu@Ubuntu:~/test$ python server_test.py 
      hello client
      hello client

    5. Stop the server with CTRL+C

      ubuntu@Ubuntu:~/$ ../target/app 
      ...
        ^C[ debug ] ../dev/app/app.cpp:27:33 [main()::<lambda(const asio::error_code&, int)>] [2025-11-24 20:42:29] (139942148851520): Received signal: 2
        [ debug ] ../dev/app/app.cpp:43:9 [int main()] [2025-11-24 20:42:29] (139942148851520): Stopping server...
        [ debug ] ../dev/app/server/server.cpp:134:9 [int Server::poll(int)] [2025-11-24 20:42:29] (139942148847296): Awakening event received
        [ debug ] ../dev/app/server/server.cpp:255:5 [void Server::run()] [2025-11-24 20:42:29] (139942148847296): Server stopped...
        [ debug ] ../dev/app/app.cpp:45:9 [int main()] [2025-11-24 20:42:29] (139942148851520): Exit.
        [ debug ] ../dev/app/server/server.cpp:60:3 [virtual Server::~Server()] [2025-11-24 20:42:29] (139942148851520): Calling ~Server
    

      

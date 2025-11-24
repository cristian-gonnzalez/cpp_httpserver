#pragma once

#include <string>
#include <iostream>
#include <utility>
#include "exception.h"


class Connection
{
  public:
    Connection(int socket, std::string host);
    virtual ~Connection();

    // Deletes copy semantics
    Connection( const Connection& other ) = delete;
    Connection& operator=( const Connection& other ) = delete;

    // Move semantics
    Connection( Connection&& rvalue ) noexcept;
    Connection& operator=( Connection&& rvalue ) noexcept;

    std::string read( int timeout = 0 );
    int write( const std::string& msg );

    void interrup();

    friend std::ostream& operator<<(std::ostream& os, const Connection& other )
    {
      os << "{ 'host': '" << other._host << "', 'id': " << other._socket << " }";
      return os;
    }

  private:
    int           _socket{-1};
    std::string   _host;

    int           _awakening_pipe[2]{-1,-1};
    
    int poll( short events, int msecs_timeout );
  };



class ConnectionException : public server::Exception
{
    public:
        explicit ConnectionException(const std::string msg)
        : server::Exception(msg)
        {
        }
};

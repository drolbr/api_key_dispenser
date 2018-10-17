#include "postgresql_wrapper.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>


int main(int argc, char* args[])
{
  char* method = getenv("REQUEST_METHOD");
  if (!method || strncmp(method, "POST", 5))
  {
    std::cout<<"Status: 405 Method not allowed\n\n";
    return 0;
  }

  try
  {
    PostgreSQL_Connection conn;
    std::cout<<"Status: 200 OK\n"
        "Content-type: text/plain\n\n";

    PostgreSQL_Result res(conn,
        ("select count(*) "
            "from keys "
            "join users on users.id = keys.user_ref "
            "where users.name = '" + std::string("bob") + "' "
                "and keys.data = '" + std::string("abcd1111") + "' "
                "and keys.revoked_ref is null").c_str());

    for (int i = 0; i < res.col_size(); ++i)
      std::cout<<'\t'<<res.col_name(i);
    std::cout<<'\n';

    for (int i = 0; i < res.row_size(); ++i)
    {
      for (int j = 0; j < res.col_size(); ++j)
        std::cout<<'\t'<<res.at(i, j);
      std::cout<<'\n';
    }
  }
  catch (const PostgreSQL_Connection::Error& e)
  {
    std::cout<<"Status: 502 DBMS unavailable\n\n";
    return 0;
  }
  catch (const PostgreSQL_Result::Error& e)
  {
    std::cout<<"Error on SQL query: "<<e.what()<<'\n';
    return 0;
  }

  return 0;
}
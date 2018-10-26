#include "../tools/cgi_helper.h"
#include "../tools/postgresql_wrapper.h"
#include "../tools/random_key.h"

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

  auto cgi_args = decode_cgi_to_plain(cgi_post_to_text());
  if (cgi_args["uid"].empty() || cgi_args["session"].empty() || cgi_args["service"].empty())
  {
    std::cout<<"Status: 400 Bad Request\n\n"
        "Parameters uid, session, and service required.\n";
    return 0;
  }

  try
  {
    if (sanitize_text(cgi_args["service"]) != cgi_args["service"])
    {
      std::cout<<"Status: 403 Forbidden\n\n"
          "Service name contains illegal characters\n";
      return 0;
    }

    PostgreSQL_Connection conn;
    PostgreSQL_Result authenticate_result(conn,
        ("select id "
            "from users "
            "join user_sessions on users.id = user_sessions.user_ref "
            "where users.id = " + sanitize_text(cgi_args["uid"]) + " "
            "and user_sessions.access_key = '" + sanitize_text(cgi_args["session"]) + "' "
            "and user_sessions.expires > now()").c_str());

    if (authenticate_result.row_size() == 0)
    {
      std::cout<<"Status: 403 Forbidden\n\n"
          "No entry found for this uid and session.\n";
      return 0;
    }
    if (authenticate_result.row_size() > 1)
    {
      std::cout<<"Status: 500 Internal error\n\n"
          "Duplicate entry for uids found.\n";
      return 0;
    }

    std::string uid = authenticate_result.at(0, 0);

    PostgreSQL_Result service_result(conn,
        ("select id, user_ref "
            "from services "
            "where uri = '" + sanitize_text(cgi_args["service"]) + "' ").c_str());

    if (service_result.row_size() == 1)
    {
      if (service_result.at(0, 1) == cgi_args["uid"])
        std::cout<<"Status: 409 Conflict\n\n"
            "The service "<<sanitize_text(cgi_args["service"])<<" already exists.\n";
      else
        std::cout<<"Status: 403 Forbidden\n\n"
            "You are not the owner of "<<sanitize_text(cgi_args["service"])<<".\n";
      return 0;
    }
    if (service_result.row_size() > 1)
    {
      std::cout<<"Status: 500 Internal error\n\n"
          "Duplicate entry for service found.\n";
      return 0;
    }

    std::string new_key = generate_random_key();

    PostgreSQL_Result insert_res(conn,
        ("insert into services (id, user_ref, uri, created, access_key) "
            "select coalesce(max(id), 0)+1, " + uid + ", '" + sanitize_text(cgi_args["service"]) + "', "
                "now(), '" + new_key + "' from services").c_str());

    std::cout<<"Status: 201 Created\n"
        "Content-type: text/plain\n\n";
    std::cout<<new_key<<'\n';
  }
  catch (const PostgreSQL_Connection::Error& e)
  {
    std::cout<<"Status: 502 DBMS unavailable\n\n";
    return 0;
  }
  catch (const PostgreSQL_Result::Error& e)
  {
    std::cout<<"Status: 502 DBMS has complained\n\n";
    std::cout<<"Error on SQL query: "<<e.what()<<'\n';
    return 0;
  }

  return 0;
}
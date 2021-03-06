#include "../tools/cgi_helper.h"
#include "../tools/postgresql_wrapper.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>


std::string generate_random_key()
{
  static bool initalized = false;
  if (!initalized)
  {
    srand(time(0));
    initalized = true;
  }

  const char* hexdigits = "0123456789abcdef";

  std::string result(40, ' ');
  for (uint i = 0; i < 40; ++i)
    result[i] = hexdigits[random()>>8 & 0xf];
  return result;
}


int main(int argc, char* args[])
{
  char* method = getenv("REQUEST_METHOD");
  if (!method || strncmp(method, "POST", 5))
  {
    std::cout<<"Status: 405 Method not allowed\n\n";
    return 0;
  }

  auto cgi_args = decode_cgi_to_plain(cgi_post_to_text());
  if (cgi_args["user"].empty() || cgi_args["oldkey"].empty())
  {
    std::cout<<"Status: 400 Bad Request\n\n"
        "Parameters user and oldkey required.\n";
    return 0;
  }

  try
  {
    PostgreSQL_Connection conn;
    PostgreSQL_Result authenticate_result(conn,
        ("select users.id, keys.service_ref "
            "from keys "
            "join users on users.id = keys.user_ref "
            "where users.name = '" + sanitize_text(cgi_args["user"]) + "' "
                "and keys.data = '" + sanitize_text(cgi_args["oldkey"]) + "' "
                "and keys.revoked_ref is null").c_str());

    if (authenticate_result.row_size() == 0)
    {
      std::cout<<"Status: 403 Forbidden\n\n"
          "No entry found for this user and key.\n";
      return 0;
    }
    if (authenticate_result.row_size() > 1)
    {
      std::cout<<"Status: 500 Internal error\n\n"
          "Duplicate entry for keys found.\n";
      return 0;
    }

    std::string user_ref = authenticate_result.at(0, 0);
    std::string service_ref = authenticate_result.at(0, 1);

    std::string new_key = generate_random_key();

    PostgreSQL_Result key_event_res(conn,
        "insert into key_events (id, happened) "
            "select max(id)+1, now() from key_events "
            "returning id");

    std::string created_ref = key_event_res.at(0, 0);

    PostgreSQL_Result res(conn,
        ("insert into keys (created_ref, service_ref, user_ref, data) "
            "values (" + created_ref + ", " + service_ref + ", " + user_ref + ", '" + new_key + "')").c_str());

    std::cout<<"Status: 200 OK\n"
        "Content-type: text/plain\n\n";
    std::cout<<new_key<<'\n';

    /*for (int i = 0; i < res.col_size(); ++i)
      std::cout<<'\t'<<res.col_name(i);
    std::cout<<'\n';

    for (int i = 0; i < res.row_size(); ++i)
    {
      for (int j = 0; j < res.col_size(); ++j)
        std::cout<<'\t'<<res.at(i, j);
      std::cout<<'\n';
    }*/
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
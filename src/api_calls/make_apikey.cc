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
        ("select id "
            "from services "
            "where uri = '" + sanitize_text(cgi_args["service"]) + "' "
            "and access_key is not null").c_str());

    if (service_result.row_size() == 0)
    {
      std::cout<<"Status: 404 Service not found\n\n"
          "No entry found for this service.\n";
      return 0;
    }
    if (service_result.row_size() > 1)
    {
      std::cout<<"Status: 500 Internal error\n\n"
          "Duplicate entry for service found.\n";
      return 0;
    }

    std::string service_id = service_result.at(0, 0);

    PostgreSQL_Result key_revoke_event_res(conn,
        "insert into key_events (id, happened) "
            "select coalesce(max(id), 0)+1, now() from key_events "
            "returning id");

    std::string revoke_event_ref = key_revoke_event_res.at(0, 0);

    PostgreSQL_Result revoke_res(conn,
        ("update keys "
            "set revoked_ref = " + revoke_event_ref + " "
            "where user_ref = " + uid + " "
            "and service_ref = " + service_id + " "
            "and revoked_ref is null").c_str());

    std::string new_key = generate_random_key();

    PostgreSQL_Result key_create_event_res(conn,
        "insert into key_events (id, happened) "
            "select coalesce(max(id), 0)+1, now() from key_events "
            "returning id");

    std::string created_ref = key_create_event_res.at(0, 0);

    PostgreSQL_Result insert_res(conn,
        ("insert into keys (created_ref, service_ref, user_ref, data) "
            "values (" + created_ref + ", " + service_id + ", " + uid + ", '" + new_key + "')").c_str());

    std::cout<<"Status: 200 OK\n"
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
    std::cout<<"Error on SQL query: "<<e.what()<<'\n';
    return 0;
  }

  return 0;
}
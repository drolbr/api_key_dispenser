#include "cgi_helper.h"
#include "postgresql_wrapper.h"
#include "random_key.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>


int main(int argc, char* args[])
{
  char* method = getenv("REQUEST_METHOD");
  if (!method || strncmp(method, "GET", 5))
  {
    std::cout<<"Status: 405 Method not allowed\n\n";
    return 0;
  }

  auto cgi_args = decode_cgi_to_plain(cgi_get_to_text());
  if (cgi_args["user"].empty() || cgi_args["uid"].empty())
  {
    std::cout<<"Status: 400 Bad Request\n\n"
        "Parameters user and uid required.\n";
    return 0;
  }

  try
  {
    std::string user_name = sanitize_text(cgi_args["user"]);
    std::string user_id = sanitize_text(cgi_args["uid"]);

    PostgreSQL_Connection conn;
    PostgreSQL_Result authenticate_result(conn,
        ("select id "
            "from users "
            "where id = " + user_id).c_str());

    if (authenticate_result.row_size() == 0)
    {
      // uid not yet known - insert user
      PostgreSQL_Result insert_user_res(conn,
          ("insert into users (id, name) "
              "values (" + user_id + ", '" + user_name + "')").c_str());
    }

    std::string session_key = generate_random_key();

    PostgreSQL_Result cleanup_session_res(conn,
        ("delete from user_sessions "
            "where user_ref = " + user_id + " "
            "and expires < now()").c_str());
    PostgreSQL_Result session_res(conn,
        ("insert into user_sessions (user_ref, access_key, expires) "
            "select " + user_id + ", '" + session_key + "', now() + interval '15 minutes'").c_str());

    PostgreSQL_Result services_result(conn,
        ("select uri "
            "from services "
            "where user_ref = " + user_id + " ").c_str());

    std::cout<<"Status: 200 OK\n"
        "Content-type: text/html; charset=utf8\n\n";

    std::cout<<
"<html>"
"<head>"
"  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" lang=\"en\"/>"
"  <meta name=\"HandheldFriendly\" content=\"true\"/>"
"  <title>OSM API key dispenser</title>"
"  <script src=\"/login_service.js\"></script>"
"</head>"
"<body onload=\"init()\">"
"  <p>Hello <strong>"<<user_name<<"</strong></p>"
""
"  <p><form action=\"#\" accept-charset=\"UTF-8\" method=post\">"
"    <select size=\"1\" id=\"service_selection\">";
    for (int i = 0; i < services_result.row_size(); ++i)
      std::cout<<"      <option>"<<services_result.at(i, 0)<<"</option>";
    std::cout<<
"    </select><br/>"
"    <input type=\"button\" value=\"Reset Access Key\" id=\"reset_service_key\"/>"
"    <input type=\"button\" value=\"Discontinue Service\" id=\"discontinue_service\"/><br/>"
"    <input type=\"text\" value=\"URI of new sevice\" id=\"new_service_uri\"/><br/>"
"    <input type=\"button\" value=\"Announce Service\" id=\"announce_service\"/><br/>"
"    <input type=\"hidden\" id=\"uid\" value=\""<<user_id<<"\"/>"
"    <input type=\"hidden\" id=\"session\" value=\""<<session_key<<"\"/>"
"  </form></p>"
""
"  <div id=\"show_keys_target\"/>"
""
"</body>"
"</html>";

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
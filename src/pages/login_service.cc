#include "../tools/base64.h"
#include "../tools/cgi_helper.h"
#include "../tools/curl_client.h"
#include "../tools/osm_permissions_reader.h"
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
  if (cgi_args["user"].empty() || cgi_args["password"].empty())
  {
    std::cout<<"Status: 400 Bad Request\n\n"
        "Parameters user and password required.\n";
    return 0;
  }

  std::string answer;

  try
  {
    std::map< std::string, std::string > headers;
    headers["Authorization"] = "Basic " + base64(cgi_args["user"] + ":" + cgi_args["password"]);

    answer = Curl_Client().send_request(
        "https://master.apis.dev.openstreetmap.org/api/0.6/user/details", headers);
  }
  catch (const Curl_Client::Error& e)
  {
    std::cout<<"Status: 502 Bad Gateway\n\n"
        "Error from background HTTPS request: "<<e.what()<<'\n';
    return 0;
  }

  User_And_Id_From_Xml user_uid(answer);
  if (user_uid.name.empty() || user_uid.uid.empty())
  {
    std::cout<<"Status: 403 Forbidden\n\n"
        "master.apis.dev.openstreetmap.org has rejected your credentials.\n";
    return 0;
  }

  try
  {
    PostgreSQL_Connection conn;
    PostgreSQL_Result authenticate_result(conn,
        ("select id "
            "from users "
            "where id = " + user_uid.uid).c_str());

    if (authenticate_result.row_size() == 0)
    {
      // uid not yet known - insert user
      PostgreSQL_Result insert_user_res(conn,
          ("insert into users (id, name) "
              "values (" + user_uid.uid + ", '" + user_uid.name + "')").c_str());
    }

    std::string session_key = generate_random_key();

    PostgreSQL_Result cleanup_session_res(conn,
        ("delete from user_sessions "
            "where user_ref = " + user_uid.uid + " "
            "and expires < now()").c_str());
    PostgreSQL_Result session_res(conn,
        ("insert into user_sessions (user_ref, access_key, expires) "
            "select " + user_uid.uid + ", '" + session_key + "', now() + interval '15 minutes'").c_str());

    PostgreSQL_Result services_result(conn,
        ("select uri "
            "from services "
            "where user_ref = " + user_uid.uid + " ").c_str());

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
"  <p>Hello <strong>"<<user_uid.name<<"</strong></p>"
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
"    <input type=\"hidden\" id=\"uid\" value=\""<<user_uid.uid<<"\"/>"
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
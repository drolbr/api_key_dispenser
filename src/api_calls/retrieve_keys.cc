#include "../tools/cgi_helper.h"
#include "../tools/postgresql_wrapper.h"

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
  if (cgi_args["service"].empty() || cgi_args["key"].empty() || cgi_args["beyond"].empty())
  {
    std::cout<<"Status: 400 Bad Request\n\n"
        "Parameters service, key, and beyond required.\n";
    return 0;
  }

  try
  {
    PostgreSQL_Connection conn;
    PostgreSQL_Result authenticate_result(conn,
        ("select id "
            "from services "
            "where uri = '" + sanitize_text(cgi_args["service"]) + "' "
                "and access_key = '" + sanitize_text(cgi_args["key"]) + "' ").c_str());

    if (authenticate_result.row_size() == 0)
    {
      std::cout<<"Status: 403 Forbidden\n\n"
          "No entry found for this service and key.\n";
      return 0;
    }
    if (authenticate_result.row_size() > 1)
    {
      std::cout<<"Status: 500 Internal error\n\n"
          "Duplicate entry for keys found.\n";
      return 0;
    }

    std::cout<<"Status: 200 OK\n"
        "Content-Type: application/xml; charset=utf-8\n\n"
        "<osm-apikeys>\n";

    std::string service_ref = authenticate_result.at(0, 0);
    int64_t beyond_id = atoll(cgi_args["beyond"].c_str());
    int64_t max_key_id = beyond_id;

    PostgreSQL_Result revoked_res(conn,
        ("select keys.data, keys.created_ref, keys.revoked_ref "
            "from keys "
            "where keys.service_ref = " + service_ref + " "
            "and keys.revoked_ref > " + sanitize_text(cgi_args["beyond"].c_str())).c_str());

    for (int i = 0; i < revoked_res.row_size(); ++i)
    {
      if ((int64_t)atoll(revoked_res.at(i, 1)) <= beyond_id)
        std::cout<<"  <revoked key=\""<<revoked_res.at(i, 0)<<"\"/>\n";
      max_key_id = std::max(max_key_id, (int64_t)atoll(revoked_res.at(i, 2)));
    }

    PostgreSQL_Result created_res(conn,
        ("select keys.data, "
                "to_char(key_events.happened, 'YYYY-MM-DD'),"
                "to_char(key_events.happened, 'HH24:MI:SS'),"
                " key_events.id "
            "from keys "
            "join key_events on keys.created_ref = key_events.id "
            "where keys.service_ref = " + service_ref + " "
            "and key_events.id > " + sanitize_text(cgi_args["beyond"].c_str()) + " "
            "and keys.revoked_ref is null").c_str());

    for (int i = 0; i < created_res.row_size(); ++i)
    {
      std::cout<<"  <apikey key=\""<<created_res.at(i, 0)<<"\""
          " created=\""<<created_res.at(i, 1)<<"T"<<created_res.at(i, 2)<<"Z\"/>\n";
      max_key_id = std::max(max_key_id, (int64_t)atoll(created_res.at(i, 3)));
    }

    std::cout<<"  <keyid max=\""<<max_key_id<<"\"/>\n"
        "</osm-apikeys>\n";
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
#ifndef CURL_CLIENT_H
#define CURL_CLIENT_H


#include <curl/curl.h>
#include <map>


struct Curl_Client
{
  struct Error : std::runtime_error
  {
    Error() : std::runtime_error("curl_easy_init() returned a nullptr") {}
    Error(const std::string& message) : std::runtime_error(message.c_str()) {}
  };

  Curl_Client();
  ~Curl_Client();

  std::string send_request(const std::string& url, const std::map< std::string, std::string >& headers);

private:
  CURL* handle;

  Curl_Client(const Curl_Client&);
  Curl_Client& operator=(const Curl_Client&);

  struct Global_Curl_Object
  {
    Global_Curl_Object() { curl_global_init(CURL_GLOBAL_DEFAULT); };
    ~Global_Curl_Object() { curl_global_cleanup(); }
  };
};


#endif

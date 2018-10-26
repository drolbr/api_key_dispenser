#include "curl_client.h"


#include <curl/curl.h>
#include <cstdio>
#include <cstring>
#include <map>


Curl_Client::Curl_Client()
{
  static Global_Curl_Object global_curl;
  handle = curl_easy_init();
  if (!handle)
    throw Error();
}


Curl_Client::~Curl_Client()
{
  if (handle)
    curl_easy_cleanup(handle);
}


namespace
{
  size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp)
  {
    std::string& result = *(std::string*)userp;
    uint old_size = result.size();
    result.resize(old_size + nmemb, ' ');
    memcpy(&result[old_size], buffer, nmemb);
    return nmemb;
  }
}


std::string Curl_Client::send_request(const std::string& url, const std::map< std::string, std::string >& headers)
{
  std::string answer;
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &write_data);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &answer);

  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

  curl_slist* slist = 0;
  for (const auto& i : headers)
    slist = curl_slist_append(slist, (i.first + ": " + i.second).c_str());
  curl_easy_setopt(handle, CURLOPT_HTTPHEADER, slist);

  CURLcode res = curl_easy_perform(handle);
  curl_slist_free_all(slist);

  if (res != CURLE_OK)
    throw Error(std::string("curl_easy_perform() failed: ") + curl_easy_strerror(res) + "\n");

  return answer;
}

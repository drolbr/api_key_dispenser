#ifndef CGI_HELPER
#define CGI_HELPER

#include <map>
#include <string>


std::string cgi_get_to_text();

std::string cgi_post_to_text();

std::map< std::string, std::string > decode_cgi_to_plain(const std::string& raw);

std::string sanitize_text(const std::string& input);


#endif

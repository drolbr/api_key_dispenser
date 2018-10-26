#include "osm_permissions_reader.h"


#include <string>


User_And_Id_From_Xml::User_And_Id_From_Xml(const std::string& xml)
{
  auto user_pos = xml.find("<user");
  if (user_pos == std::string::npos)
    return;

  auto tag_closing_pos = xml.find(">", user_pos);
  if (tag_closing_pos == std::string::npos)
    return;

  auto uid_start_pos = xml.find("id=\"", user_pos);
  if (uid_start_pos == std::string::npos || tag_closing_pos < uid_start_pos)
    return;

  auto uid_end_pos = xml.find("\"", uid_start_pos+4);
  if (uid_end_pos == std::string::npos || tag_closing_pos < uid_end_pos)
    return;

  auto name_start_pos = xml.find("display_name=\"", user_pos);
  if (name_start_pos == std::string::npos || tag_closing_pos < name_start_pos)
    return;

  auto name_end_pos = xml.find("\"", name_start_pos+14);
  if (name_end_pos == std::string::npos || tag_closing_pos < name_end_pos)
    return;

  uid = xml.substr(uid_start_pos + 4, uid_end_pos - uid_start_pos - 4);
  name = xml.substr(name_start_pos + 14, name_end_pos - name_start_pos - 14);
}

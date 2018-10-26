#ifndef OSM_PERMISSION_READER_H
#define OSM_PERMISSION_READER_H


#include <string>


struct User_And_Id_From_Xml
{
  User_And_Id_From_Xml(const std::string& xml);

  std::string name;
  std::string uid;
};


#endif

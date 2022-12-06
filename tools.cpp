#include "tools.h"
#include <direct.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

bool tools::recursive_create_directory(std::string path) {
  std::vector<std::string> paths;
  std::stringstream ss(path);
  std::string temp;
  char delimeter;
#ifdef _WIN32
  delimeter = '\\';
#else
  delimeter = '/';
#endif
  int index = 0;
  while (std::getline(ss, temp, delimeter)) {
    if (index == 0 and temp == "")
      paths.push_back(std::to_string(delimeter));
    else if (temp!="")
      paths.push_back(temp);
  }
  path = "";
  for (auto i : paths) {
    path += i + delimeter;
    std::clog<<path<<std::endl;
    _mkdir(path.c_str());
  }
  return false;
}
#include "tools.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>


#define DEBUG false

const std::string dump_folder_name = "./data/login_register/";

const std::string dump_file_name = "user_info.db";

using namespace std;

// using handler = void (*)(const map<string, string> &, string, string);

typedef void (*handler)(const map<string, string> &, string, string);

// struct UserInfo{
//     string username;
//     string password;
// };

map<string, string> load_existed_users() {
  map<string, string> existed_users;
  ifstream input_file;
  input_file.open(dump_folder_name + dump_file_name);
  if (!input_file.is_open()) {
    cout << "Open file failed!";
    tools::recursive_create_directory(dump_folder_name);
    exit(1);
  }
  string username, password;
  string line, temp;
  while (getline(input_file, line)) {
    clog << "Log: " << line << endl;
    stringstream ss_line(line);
    int index = 0;
    while (getline(ss_line, temp, ':')) {
      clog << "Log: " << temp << endl;
      if (index == 0) {
        username = temp;
      } else if (index == 1) {
        existed_users[username] = temp;
      } else {
        cout << "Parse file error!";
        exit(1);
      }
      index += 1;
    }
  }
  input_file.close();
  return existed_users;
}

void user_register(const map<string, string> &existed_users, string username,
                   string password) {
  if (existed_users.count(username)) {
    cout << "Username already registered!";
    exit(1);
  }
  ofstream output_file;
  output_file.open(dump_folder_name + dump_file_name,
                   ios_base::in | ios_base::app);
  if (!output_file.is_open()) {
    cout << "Open file failed!";
    exit(1);
  }
  output_file << username << ":" << password << endl;
  output_file.close();
  cout << "Register Successfully!";
}

void user_login(const map<string, string> &existed_users, string username,
                string password) {
  try {
    const std::string &stored_password = existed_users.at(username);
    if (stored_password == password) {
      cout << "Login Successfully!";
    } else {
      cout << "Username or Password Error!";
    }
  } catch (const std::out_of_range &) {
    cout << "Username not existed!";
    exit(1);
  }
}

int main(int argc, char const *argv[]) {
  if (!DEBUG) {
    std::ofstream nullstream;
    std::clog.rdbuf(nullstream.rdbuf());
  }
  map<int, string> choices = {{1, "Register"}, {2, "Login"}};
  map<int, handler> handlers = {{1, user_register}, {2, user_login}};
  map<string, map<string, string>> users;
  for (auto i : choices) {
    cout << i.first << ": " << i.second << endl;
  }
  cout << "Please Select a Choice:  \b";
  int choice;
  cin >> choice;
  if (!cin) {
    cin.clear();
    string illegal_content;
    cin >> illegal_content;
    cout << "Error: Un-Recongnized Input: " << illegal_content;
    exit(1);
  }
  while (cin.get() != '\n') {
    continue;
  }

  string username, password;

  cout << "Please input username: ";
  cin >> username;

  cout << "Please input password: ";
  cin >> password;

  map<string, string> &&existed_users = load_existed_users();

  for (auto i : existed_users) {
    clog << "Log: " << i.first << "-" << i.second << endl;
  }

  handlers[choice](existed_users, username, password);

  return 0;
}
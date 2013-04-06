#include <iostream>
#include <sstream>
#include <string>

namespace ratpoison {

enum EXCEPTION {
  ENV_UNDEFINED,
  BAD_WS_INDEX,
  BAD_PIPE,
  THATS_NOT_GOOD
};

std::string call(std::string const&);

std::string getenv(std::string const&);

int getenv_int(std::string const&);

template<typename T>
void setenv(std::string const& var, T const& value){
  std::stringstream ss;
  ss << "setenv " << var << ' ' << value;
  call(ss.str());
}

void unsetenv(std::string const&);

bool is_env(std::string const&);

void echo(std::string const&);

std::string fdump();

std::string string_num(std::string const&, unsigned);

inline std::string num_on_str(unsigned num, std::string const& str){
  std::stringstream ss;
  ss << str << num;
  return ss.str();
}


inline bool group_is_empty(){
  // "No managed windows"?
  return call("windows")[0]=='N';
}


}


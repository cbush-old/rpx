#include <cstdio>
#include "rp_util.h"

using std::string;
using std::stringstream;
using std::cout;

string trim(string s){
  
  auto b = s.find_first_not_of(" \t\n");
  
  if(b==std::string::npos)
    return "";
  
  auto e = s.find_last_not_of(" \t\n");
  auto l = e - b + 1;
  return s.substr(b, l);

}

namespace ratpoison {

string call(string const& cmd){

  stringstream ss;
  ss << "ratpoison -c '" << cmd  << "'";

  string result;
  
  FILE* p = popen(ss.str().c_str(), "r");

  if(p==NULL) throw BAD_PIPE;

  const size_t SIZE = 256;
  char buffer[SIZE];
  
  while(!feof(p)){
    if(fgets(buffer,SIZE,p)!=NULL)
      result += buffer;
  }

  pclose(p);

  return trim(result);

}

string getenv(string const& var){

  return call(string("getenv ") + var);
  
}

int getenv_int(string const& var){
  
  stringstream ss;
  int out;
  ss << call(string("getenv ") + var);

  if(ss >> out)
    return out;
  
  throw ENV_UNDEFINED;

}

void unsetenv(string const& var){
  call(string("unsetenv ") + var);
}

string fdump(){
  return call("fdump");
}

bool is_env(string const& var){
  return call(string("getenv " + var))!="";
}

void echo(string const& what){
  call(string("echo ") + what);
}

} // namespace ratpoison

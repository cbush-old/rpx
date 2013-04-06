#include <vector>
#include "rp_util.h"

using std::cout;
using std::string;
using std::vector;
using std::stringstream;

using namespace ratpoison;

unsigned n_ws { 0 }, curr_ws { 0 };

inline void get_curr_ws(){
  curr_ws = getenv_int("rpmd_curr_ws");
}

inline void set_curr_ws(unsigned i){

  curr_ws = i;
  setenv("rpmd_curr_ws", i);
  echo(num_on_str(i+1, "Workspace ") + num_on_str(n_ws,"/"));

}

void make_ws(){

  call(num_on_str(n_ws, "gnew rpmd_ws"));
  call("select -");
  call("only");
  setenv(num_on_str(n_ws, "rpmd_fdata_ws"), fdump());
  setenv("rpmd_n_ws", ++n_ws);
  set_curr_ws(n_ws - 1);
  
}

void save_ws(){
  
  string var = num_on_str(curr_ws, "rpmd_fdata_ws");
  string fdata = fdump();
  setenv(var, fdata);

}

void load_ws(unsigned i){
  
  string var = num_on_str(i, "rpmd_fdata_ws");
  string fdata = getenv(var);
  call(string("frestore ") + fdata);
  call(num_on_str(i, "gselect rpmd_ws"));
  set_curr_ws(i);
  
}

void delete_ws(){

  call("gdelete");
  
  for(unsigned i = curr_ws + 1; i < n_ws; ++i){

    call(num_on_str(i, "gselect rpmd_ws"));
    call(num_on_str(i - 1, "grename rpmd_ws"));
    
    setenv(
      num_on_str(i - 1, "rpmd_fdata_ws"), 
      getenv(num_on_str(i, "rpmd_fdata_ws"))
    );

  }

  setenv("rpmd_n_ws", --n_ws);
  //unsetenv(num_on_str(n_ws, "rpmd_fdata_ws"));    
  
}

void prev_ws(){
  
  save_ws();

  if(curr_ws == 0){
    
    unsigned i = n_ws - 1;
    
    call(num_on_str(n_ws - 1, "gselect rpmd_ws"));
    
    if(group_is_empty())
      load_ws(i);
    else
      make_ws();
  
  } else {

    load_ws(curr_ws - 1);
  
  }

}

void next_ws(){
  
  save_ws();

  if(curr_ws == n_ws - 1){
    if(group_is_empty()){
      // wrap to first ws
      load_ws(0);
  
    } else {
      
      make_ws();
    
    }
  } else {    
    
    load_ws(curr_ws + 1);
  
  }

}

int main(int argc, char* argv[]){

  if(is_env("rpmd_n_ws")){
    // restore session
    
    n_ws = getenv_int("rpmd_n_ws");
    get_curr_ws();
    
    vector<string> args (argv + 1, argv + argc);
    
    for(auto& arg : args){
      if(arg=="next") next_ws();
      else if(arg=="prev") prev_ws();
      else if(arg=="delete"){
        if(group_is_empty()){
          if(n_ws > 1){
            delete_ws();
            load_ws(curr_ws);
          } else {
            echo("Can't delete only ws");
          }
        } else {
          echo("Group must be empty");
        }
      }
    }

  } else {
    // new session
    make_ws();
    call("gmerge default");
    call("gdelete default");
    
    call("definekey top s-Right exec rpmd next");
    call("definekey top s-Left exec rpmd prev");
    call("definekey top s-BackSpace exec rpmd delete");
    
    // add a hook to close window event to update groups
    // call("addhook deletewindow exec rpmd dw");
  
  }

}



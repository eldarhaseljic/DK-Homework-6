#ifndef ENV_HPP
#define ENV_HPP

#include <map>
#include <string>
#include <stack>

struct FunctionNode;

struct Environment {
  std::map<std::string, int> variables_;
  std::map<std::string, FunctionNode*> functions_;
 
  //////////////////////////////////////////
  //Dodatni zadatal - environment stack
  std::stack < std::map < std::string, int > > funcBlok_;

  std::stack < std::map < std::string, int > >& funcBlok(){
      return funcBlok_;
  }
  
  ///////////////////////////////////////////
  std::map<std::string, int>& variables() {
    return variables_;
  };

  std::map<std::string, FunctionNode*>& functions() {
    return functions_;
  };

};

#endif // ENV_HPP

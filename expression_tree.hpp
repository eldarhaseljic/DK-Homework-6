#ifndef EXPRESSION_TREE_H
#define EXPRESSION_TREE_H

#include <iostream>
#include <string>
#include <vector>

#include "environment.hpp"

// Interface type for a node
struct ExpressionNode {
  virtual ~ExpressionNode(){};
  virtual int evaluate(Environment& env) = 0;
};

struct NumberNode : public ExpressionNode {
  NumberNode(int v) : value{v} {}

  int evaluate(Environment& env) override { return value; }

  private:
  int value;
};

//Zadatak 2 drugi dio 
struct VariableNode : public ExpressionNode {
  VariableNode(std::string s) : id{std::move(s)} {}

  int evaluate(Environment& env) override { return env.variables()[id]; }

  private:
  std::string id;
};

////////////////////////////////////
//Zadatak 5
struct FunctionNode : public ExpressionNode {
  FunctionNode(std::vector<std::string> argument_names,
               std::vector<ExpressionNode*> body)
      : argument_names{std::move(argument_names)}, body{body} {}

  int evaluate(Environment& env) override {
    for (auto expr : body) {
      expr->evaluate(env);
    }
    return 0;
  }

  std::vector<std::string> argument_names;
  std::vector<ExpressionNode*> body;
};

struct FunctionDefinitionNode : public ExpressionNode {
  FunctionDefinitionNode(std::string function_name,
                         std::vector<std::string> argument_names,
                         std::vector<ExpressionNode*> body)
      : function_name{std::move(function_name)},
        argument_names{std::move(argument_names)},
        body{body} {}
  int evaluate(Environment& env) override {
    env.functions()[function_name] = new FunctionNode(std::move(argument_names), std::move(body));
    return 0;
  }

  std::vector<std::string> argument_names;
  std::string function_name;
  std::vector<ExpressionNode*> body;
};

struct FunctionCallNode : public ExpressionNode {
  FunctionCallNode(std::string function_name,
                   std::vector<ExpressionNode*> argument_values)
      : function_name{std::move(function_name)},
        argument_values{std::move(argument_values)} {}
  
  int evaluate(Environment& env) override {
    ///////////////////////////////////////
    //Dodatni zadatak - environment stack
    env.funcBlok().push(env.variables());

    ////////////////////////////////
    //Zadatak 5
    auto it = env.functions().find(function_name);
    if(it!=env.functions().end())
    {
      for(auto i = 0; i<it->second->argument_names.size();++i)
      {
        auto name = it->second->argument_names.at(i);
        env.variables()[name] = argument_values.at(i)->evaluate(env); 
      }
      
    it->second->evaluate(env);
    
    ////////////////////////////////
    //Dodatni zadatak - environment stack
      env.variables() = env.funcBlok().top();
      env.funcBlok().pop();
    }
    return 0;
  }

  std::string function_name;
  std::vector<ExpressionNode*> argument_values;
};

//////////////////////////////////////////////////
struct PlusNode : public ExpressionNode {
  PlusNode(ExpressionNode* lhs, ExpressionNode* rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment& env) override {
    return left->evaluate(env) + right->evaluate(env);
  }

  private:
  ExpressionNode* left = nullptr;
  ExpressionNode* right = nullptr;
};

struct MinusNode : public ExpressionNode {
  MinusNode(ExpressionNode* lhs, ExpressionNode* rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment& env) override {
    return left->evaluate(env) - right->evaluate(env);
  }

  private:
ExpressionNode* left = nullptr;
  ExpressionNode* right = nullptr;
};

//Zadatak 1
struct EQNode : public ExpressionNode {
  EQNode(ExpressionNode* lhs, ExpressionNode* rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment& env) override {
    return ((left -> evaluate(env)) == (right -> evaluate(env)));
  }

  private:
  ExpressionNode* left = nullptr;
  ExpressionNode* right = nullptr;
};

struct NENode : public ExpressionNode {
  NENode(ExpressionNode* lhs, ExpressionNode* rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment& env) override {
    return ((left -> evaluate(env)) != (right -> evaluate(env)));
  }

  private:
  ExpressionNode* left = nullptr;
  ExpressionNode* right = nullptr;
};

struct LessNode : public ExpressionNode {
  LessNode(ExpressionNode* lhs, ExpressionNode* rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment& env) override {
    return ((left -> evaluate(env)) < (right -> evaluate(env)));
  }

  private:
  ExpressionNode* left = nullptr;
  ExpressionNode* right = nullptr;
};

struct GreaterNode : public ExpressionNode {
  GreaterNode(ExpressionNode* lhs, ExpressionNode* rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment& env) override {
    return ((left -> evaluate(env)) > (right -> evaluate(env)));
  }

  private:
  ExpressionNode* left = nullptr;
  ExpressionNode* right = nullptr;
};

struct DivideNode : public ExpressionNode {
  DivideNode(ExpressionNode* lhs, ExpressionNode* rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment& env) override {
    return left->evaluate(env) / right->evaluate(env);
  }

  private:
  ExpressionNode* left = nullptr;
  ExpressionNode* right = nullptr;
};

struct MultiplyNode : public ExpressionNode {
  MultiplyNode(ExpressionNode* lhs, ExpressionNode* rhs) : left{lhs}, right{rhs} {}
  int evaluate(Environment& env) override {
    return left->evaluate(env) * right->evaluate(env);
  }

  private:
  ExpressionNode* left = nullptr;
  ExpressionNode* right = nullptr;
};

///////////////////////////////////////////
//Zadatak 2 prvi dio
struct AssignmentNode : public ExpressionNode {
  AssignmentNode(const char* id, ExpressionNode* rhs) : id{id}, right{rhs} {}
  int evaluate(Environment& env) override {
    auto val = right->evaluate(env);
    //U odnosu na lab 8 ovdje imamo .variables() zato sto u
    //environment.hpp imamo dvije mape sada 
    //tj jedna od njih je za varijable 
    env.variables()[id] = val;
    return val;
  }

  std::string id;
  ExpressionNode* right = nullptr;
};

////////////////////////////////////////////////
//Zadatak 3
struct IfNode : public ExpressionNode {
  IfNode(ExpressionNode* cond, ExpressionNode* body)
      : condition{cond}, body{body} {}
  int evaluate(Environment& env) override {
    if (condition->evaluate(env)) return body->evaluate(env);
    return 0;
  }

  private:
  ExpressionNode* condition = nullptr;
  ExpressionNode* body = nullptr;
};

struct IfElseNode : public ExpressionNode {
  IfElseNode(ExpressionNode* cond, ExpressionNode* body, ExpressionNode* elseBody)
    : condition{cond}, body{body}, elseBody{elseBody} {}
  int evaluate(Environment& env) override {
    if(condition->evaluate(env)) return body->evaluate(env);
    else return elseBody->evaluate(env);
  }

  private:
  ExpressionNode* condition = nullptr;
  ExpressionNode* body = nullptr;
  ExpressionNode* elseBody = nullptr;
};

////////////////////////////////////////////////
struct PrintNode : public ExpressionNode {
  PrintNode(ExpressionNode* value) : value{value} {}

  int evaluate(Environment& env) override {
    printf("> %d\n", value->evaluate(env));
    return 0;
  }

  ExpressionNode* value = nullptr;
};

///////////////////////////////////////
//Zadatak 4
struct WhileNode : public ExpressionNode {
  WhileNode(ExpressionNode* cond, std::vector<ExpressionNode*> body)
      : condition{cond}, body{body} {}
  int evaluate(Environment& env) override {
    while(condition->evaluate(env))
    {
      for(ExpressionNode * node: body)
      {
        node -> evaluate(env);
      }
    }
    return 0;
  }

  private:
  ExpressionNode* condition = nullptr;
  std::vector<ExpressionNode*> body;
};

/////////////////////////////////////////////
#endif  // EXPRESSION_TREE_H

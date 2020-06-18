#pragma once

#include <string>
#include <vector>
#include "formula-tree/formula-tree.h"

inline std::string nextId(std::string id) { return "next_" + id; }
inline std::string asub(std::string op1, std::string op2) { return (op1 == "") ? op2 : ((op2 == "") ? op1 : (op1 + " - " + op2)); }
inline std::string aadd(std::string op1, std::string op2) { return (op1 == "") ? op2 : ((op2 == "") ? op1 : (op1 + " + " + op2)); }
inline std::string amul(std::string op1, std::string op2) { return (op1 == "") ? op2 : ((op2 == "") ? op1 : (op1 + " * " + op2)); }
inline std::string adiv(std::string op1, std::string op2) { return (op1 == "") ? op2 : ((op2 == "") ? op1 : (op1 + " / " + op2)); }
inline std::string land(std::string op1, std::string op2) { return (op1 == "") ? op2 : ((op2 == "") ? op1 : (op1 + " && " + op2)); }
inline std::string lor(std::string op1, std::string op2) { return (op1 == "") ? op2 : ((op2 == "") ? op1 : (op1 + " || " + op2));  }
inline std::string limplies(std::string op1, std::string op2) { return (op1 == "") ? op2 : ((op2 == "") ? op1 : (op1 + " -> " + op2));  }
inline std::string ldimplies(std::string op1, std::string op2) { return (op1 == "") ? op2 : ((op2 == "") ? op1 : (op1 + " <-> " + op2));  }
inline std::string parenthesize(std::string op1) { return (op1 == "") ? op1 : "(" + op1 + ")";  }
inline std::string leq(std::string op1, std::string op2) { return (op1 == "") ? op2 : ((op2 == "") ? op1 : (op1 + " == " + op2)); }
inline std::string lnot(std::string op1) { return "!" + op1; }
inline std::string lle(std::string op1, std::string op2) { return (op1 == "") ? op2 : ((op2 == "") ? op1 : (op1 + " <= " + op2)); }
inline std::string lge(std::string op1, std::string op2) { return (op1 == "") ? op2 : ((op2 == "") ? op1 : (op1 + " >= " + op2)); }

inline std::string exactlyOneTrue(std::vector<std::string> clauses) {
  std::string form = "";
  for(int i = 0; i < clauses.size(); ++i) {
    std::string clause = clauses[i];
    for(int j = 0; j < clauses.size(); ++j) {
      if(j != i) {
       clause = land(clause, lnot(clauses[j]));
      }
    }
    clause = parenthesize(clause);
    if(i == 0) { form = clause; }
    else { form = lor(form, clause); }
  }
  return parenthesize(form);
}

inline std::string exactlyOneTruePb(std::vector<std::string> clauses) {
  std::string form = "";
  for(int i = 0; i < clauses.size(); ++i) {
    if(i != clauses.size() - 1) { form = form + clauses[i] + " + "; }
    else {  form = parenthesize(form + clauses[i]) + " == 1 "; } 
  }
  return parenthesize(form);
}

inline std::string allTrue(std::vector<std::string> clauses) {
  std::string form = "";
  for(auto clause : clauses) {
    if(form == "") { form = clause; }
    else { form = land(form, clause); }
  } 
  return form;
}


inline std::string substitute(std::string formula, std::map<std::string, std::string> mapper) {
  if(mapper.size() == 0) { return formula; }
  FormulaTree tree(formula);
  tree.substitute(mapper);
  return tree.getFormula();
}



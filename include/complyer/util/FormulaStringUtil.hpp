#pragma once

#include <string>
#include <vector>

inline std::string nextId(std::string id) { return "next_" + id; }
inline std::string land(std::string op1, std::string op2) { return (op1 + " && " + op2); }
inline std::string lor(std::string op1, std::string op2) { return (op1 + " || " + op2);  }
inline std::string limplies(std::string op1, std::string op2) { return (op1 + " -> " + op2);  }
inline std::string parenthesize(std::string op1) { return "(" + op1 + ")";  }
inline std::string leq(std::string op1, std::string op2) { return (op1 + " == " + op2); }
inline std::string lnot(std::string op1) { return "!" + op1; }
inline std::string lle(std::string op1, std::string op2) { return (op1 + "<=" + op2); }
inline std::string lge(std::string op1, std::string op2) { return (op1 + ">=" + op2); }

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
  return form;
}

inline std::string allTrue(std::vector<std::string> clauses) {
  std::string form = "";
  for(auto clause : clauses) {
    if(form == "") { form = clause; }
    else { form = land(form, clause); }
  } 
  return form;
}

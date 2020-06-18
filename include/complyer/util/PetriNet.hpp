#pragma once

#include <vector>
#include <string>

class PetriNet {
  public:

    PetriNet() {  }
    PetriNet(std::vector<std::string> initial_marking, std::vector<std::vector<std::string>> incidence_matrix) :
      incidence_matrix(incidence_matrix),
      initial_marking(initial_marking) {  }

    inline std::vector<std::string> getInitialMarking() { return initial_marking; }
    inline std::vector<std::vector<std::string>> getIncidenceMatrix() { return incidence_matrix;  }

    inline void setInitialMarking(std::vector<std::string> initial_marking) { this->initial_marking = initial_marking; }
    inline void setIncidenceMatrix(std::vector<std::vector<std::string>> incidence_matrix) { this->incidence_matrix = incidence_matrix; }
  
  private:

    std::vector<std::string> initial_marking;
    std::vector<std::vector<std::string>> incidence_matrix;
};


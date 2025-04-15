#ifndef INFORM_PROD_TERM_HPP
#define INFORM_PROD_TERM_HPP

#include "inform.hpp"

//!product term
struct inform::ProdTerm {
   private:
      uint32 _varVals; //value of variables
      uint32 _varsMask; //which variables are actually part of the term
   public:
      ProdTerm(uint32 vars = 0, uint32 mask = 0):_varVals{vars},_varsMask{mask} {}
      static ProdTerm makeRand();

} alignas(uint64);

#endif
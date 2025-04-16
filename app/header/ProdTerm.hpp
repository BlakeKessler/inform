#ifndef INFORM_PROD_TERM_HPP
#define INFORM_PROD_TERM_HPP

#include "inform.hpp"

//!product term
struct inform::ProdTerm {
   public:
      enum Status : ubyte {
         FALSE = false,
         TRUE = true,
         NULL
      };

   private:
      uint32 _varVals; //value of variables
      uint32 _varsMask; //which variables are actually part of the term
      //!_varsMask -> (_varVals ? CONTRADICTION : TAUTOLOGY)
   public:
      ProdTerm(uint32 vars = 0, uint32 mask = 0):_varVals{vars & mask},_varsMask{mask} {}
      ProdTerm copy() const { return self; }
      static ProdTerm makeRand();

      Status operator[](ubyte i);

      ProdTerm& operator&=(const ProdTerm& other);
      ProdTerm operator&(const ProdTerm& other) const { return copy() &= other; }
      SopExpr operator|(const ProdTerm& other) const;

} alignas(uint64);

#endif
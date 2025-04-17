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
      uint32 _vals; //value of variables
      uint32 _mask; //which variables are actually part of the term
      //!_mask -> (_vals ? CONTRADICTION : TAUTOLOGY)
   public:
      ProdTerm(uint32 vars = 0, uint32 mask = 0):_vals{vars & mask},_mask{mask} {}
      ProdTerm copy() const { return self; }
      static ProdTerm makeRand();

      Status operator[](ubyte i);

      ProdTerm& operator&=(const ProdTerm& other);
      ProdTerm operator&(const ProdTerm& other) const { return copy() &= other; }
      SopExpr operator|(const ProdTerm& other) const;

      bool isContradiction() const;
      bool isTautology() const;

      bool operator==(const ProdTerm& other) const;
      bool implies(const ProdTerm& other) const;

} alignas(uint64);

#endif
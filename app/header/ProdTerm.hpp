#ifndef INFORM_PROD_TERM_HPP
#define INFORM_PROD_TERM_HPP

#include "inform.hpp"
#include <bit>

//!product term
struct alignas(uint64) inform::ProdTerm {
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
      static ProdTerm makeRand(uint maxVars, uint sparsity);

      uint32 vals() const { return _vals; }
      uint32 mask() const { return _mask; }

      uint64 toInt() const { return std::bit_cast<uint64>(self); }
      uint64 toUint() const { return std::bit_cast<uint64>(self); }
      sint64 toSint() const { return std::bit_cast<sint64>(self); }

      Status operator[](ubyte i) const;

      ProdTerm& operator&=(const ProdTerm& other);
      ProdTerm operator&(const ProdTerm& other) const { return copy() &= other; }
      SopExpr operator|(const ProdTerm& other) const;

      bool isContradiction() const;
      bool isTautology() const;

      bool operator==(const ProdTerm& other) const;
      bool implies(const ProdTerm& other) const;

};

namespace mcsl {
   uint writef(File&, const inform::ProdTerm&, char, FmtArgs);
};

#endif
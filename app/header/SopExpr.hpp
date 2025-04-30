// A class representing a "sum of products expression" (disjunctive normal form)
// It is basically just a wrapper around a vector of product terms, with special methods for pushing and popping objects to try to keep expressions simplified

#ifndef INFORM_SOP_EXPR_HPP
#define INFORM_SOP_EXPR_HPP

#include "inform.hpp"

#include "ProdTerm.hpp"

#include "dyn_arr.hpp"
#include "arr_span.hpp"

//!Sum-of-products Expression
class inform::SopExpr {
   private:
      //!TODO: maybe add a _baseTerm? maybe make a tree-like structure?
      mcsl::dyn_arr<ProdTerm> _terms;

      SopExpr(const SopExpr& lhs, const SopExpr& rhs); //AND implementation constructor

      SopExpr& normalize();
      uint normalizedPush(ProdTerm term, uint index);
      uint normalizedPushBack(ProdTerm term);
      void pop(uint index);
      void swap(uint lhs, uint rhs);
   public:
      SopExpr():_terms{} {}
      SopExpr(mcsl::arr_span<ProdTerm> terms):_terms{terms} { normalize(); }
      SopExpr(SopExpr&& other):_terms(std::move(other._terms)) {}
      SopExpr(const SopExpr& other):_terms(other._terms) {}

      //!randomly generate a SopExpr object
      //!had to be a constructor instead of a static method because of memory management
      //!for explanations of the parameters, see the help menu (run the executable with no parameters)
      SopExpr(uint termCount, uint maxVars, uint sparsity);
      
      SopExpr move() { return std::move(self); }
      SopExpr copy() const { return self; }
      
      SopExpr& operator=(SopExpr&& other) = default;
      SopExpr& operator=(const SopExpr& other) = default;

      SopExpr& operator|=(const ProdTerm& term);
      SopExpr& operator|=(const SopExpr& other);
      SopExpr& operator&=(const ProdTerm& term);
      SopExpr& operator&=(const SopExpr& other);
      
      SopExpr operator|(const ProdTerm& other);
      SopExpr operator|(const SopExpr& other);
      SopExpr operator&(const ProdTerm& other);
      SopExpr operator&(const SopExpr& other);

      ProdTerm* begin() { return _terms.begin(); }
      ProdTerm* end() { return _terms.end(); }
      const ProdTerm* begin() const { return _terms.begin(); }
      const ProdTerm* end() const { return _terms.end(); }

      bool isContradiction() const;
      bool isTautology() const;
};

namespace mcsl {
   uint writef(File&, const inform::SopExpr&, char, FmtArgs);
};

#endif
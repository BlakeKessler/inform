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

   public:
      SopExpr():_terms{} {}
      SopExpr(mcsl::arr_span<ProdTerm> terms):_terms{terms} {}
      SopExpr(SopExpr&& other):_terms(std::move(other._terms)) {}
      SopExpr(const SopExpr& other):_terms(other._terms) {}
      SopExpr&& move() { return std::move(self); }
      SopExpr copy() const { return self; }
      static SopExpr makeRand(uint termCount);
      
      SopExpr& operator=(SopExpr&& other) = default;
      SopExpr& operator=(const SopExpr& other) = default;

      SopExpr& normalize();

      SopExpr& operator|=(const ProdTerm& term);
      SopExpr& operator|=(const SopExpr& other);
      SopExpr& operator&=(const ProdTerm& term);
      SopExpr& operator&=(const SopExpr& other);
      
      SopExpr operator|(const ProdTerm& other);
      SopExpr operator|(const SopExpr& other);
      SopExpr operator&(const ProdTerm& other);
      SopExpr operator&(const SopExpr& other);
};

#endif
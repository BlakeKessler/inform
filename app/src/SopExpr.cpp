#ifndef SOP_EXPR_CPP
#define SOP_EXPR_CPP

#include "SopExpr.hpp"

inform::SopExpr inform::SopExpr::makeRand(uint termCount) {
   SopExpr expr{};
   while (termCount--) {
      expr.push_back(ProdTerm::makeRand());
   }
   return expr;
}

inform::SopExpr& inform::SopExpr::operator|=(const ProdTerm& term) {
   push_back(term);
   return self;
}
inform::SopExpr& inform::SopExpr::operator|=(const SopExpr& other) {
   for (const auto& term : other) {
      push_back(term);
   }
   return self;
}

inform::SopExpr& inform::SopExpr::operator&=(const ProdTerm& term) {
   for (auto& t : _terms) {
      t &= term;
   }
}
inform::SopExpr& inform::SopExpr::operator&=(const SopExpr& other) {
   self = (self & other).move();
}

inform::SopExpr inform::SopExpr::operator|(const ProdTerm& other) {
   return (copy() |= other).move();
}
inform::SopExpr inform::SopExpr::operator|(const SopExpr& other) {
   return (copy() |= other).move();
}
inform::SopExpr inform::SopExpr::operator&(const ProdTerm& other) {
   return (copy() &= other).move();
}
inform::SopExpr inform::SopExpr::operator&(const SopExpr& other) {
   SopExpr expr{};
   for (const auto& i : _terms) {
      for (const auto& j : other._terms) {
         expr.push_back(i & j);
      }
   }
   return expr.move();
}

#endif
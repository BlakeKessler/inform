#ifndef SOP_EXPR_CPP
#define SOP_EXPR_CPP

#include "SopExpr.hpp"

inform::SopExpr inform::SopExpr::makeRand(uint termCount) {
   SopExpr expr{};
   while (expr.size() < termCount) {
      const ProdTerm newTerm = ProdTerm::makeRand();
      if (newTerm.isContradiction() || newTerm.isTautology()) { //filter out contradictions and tautologies
         continue;
      }
      
      //find the location for the new term
      for (uint i = 0; i < expr._terms.size(); ++i) {
         if (newTerm.implies(expr._terms[i])) { //new term implies term i, so it can replace it
            expr._terms[i] = newTerm;

            //remove redundant terms
            ++i;
            while (i < expr._terms.size()) {
               if (newTerm.implies(expr._terms[i])) { //new term implies term i, so it can be removed
                  expr._terms[i] = expr._terms.back();
                  expr._terms.pop_back();
                  if (i == expr._terms.size()) { [[unlikely]];
                     goto AFTER_FINALLY;
                  }
                  continue;
               }
               ++i;
            }
         }
      }
      // finally {
      expr._terms.push_back(newTerm);
      // }
      AFTER_FINALLY:
   }
   return expr.move();
}

inform::SopExpr& inform::SopExpr::normalize() {
   for (uint i = 1; i < _terms.size(); ++i) {
      for (uint j = 0; j < i;) {
         if (_terms[j].implies(_terms[i])) {
            _terms[i] = _terms.back();
            _terms.pop_back();
            if (i == _terms.size()) { [[unlikely]];
               return self;
            }
            j = 0;
            continue;
         }
         ++j;
      }
   }
   return self;
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
   for (uint i = 0; i < _terms.size();) {
      _terms[i] &= term;
      if (_terms[i].isContradiction()) {
         _terms[i] = _terms.back();
         _terms.pop_back();
         if (i == _terms.size()) { [[unlikely]];
            break;
         }
         continue;
      }
      ++i;
   }
   return normalize();
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
         const ProdTerm tmp = i & j;
         if (!tmp.isContradiction()) {
            expr.push_back(tmp);
         }
      }
   }
   expr.normalize();
   return expr.move();
}

#endif
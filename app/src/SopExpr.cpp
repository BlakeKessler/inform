#ifndef SOP_EXPR_CPP
#define SOP_EXPR_CPP

#include "SopExpr.hpp"

#include "io.hpp"

//!generate a random sum-of-products expression
inform::SopExpr::SopExpr(uint termCount) {
   while (_terms.size() < termCount) {
      const ProdTerm newTerm = ProdTerm::makeRand();

      //filter out contradictions and tautologies
      if (newTerm.isContradiction() || newTerm.isTautology()) {
         continue;
      }
      
      //find the location for the new term
      for (uint i = 0; i < _terms.size(); ++i) {
         if (newTerm.implies(_terms[i])) { //new term implies term i, so it can replace it
            _terms[i] = newTerm;

            //remove redundant terms
            ++i;
            while (i < _terms.size()) {
               if (newTerm.implies(_terms[i])) { //new term implies term i, so it can be removed
                  //move back term over term i
                  _terms[i] = _terms.back();
                  _terms.pop_back();

                  if (i == _terms.size()) { [[unlikely]]; //break out of for loop and skip pushing the new node (since it has already overwritten a redundant term)
                     goto AFTER_FINALLY;
                  }
                  //skip increment
                  continue;
               }
               ++i;
            }
         }
      }
      // finally {
      _terms.push_back(newTerm);
      // }
      AFTER_FINALLY:
   }
}
inform::SopExpr::SopExpr(uint termCount, uint maxVars, uint sparsity) {
   while (_terms.size() < termCount) {
      const ProdTerm newTerm = ProdTerm::makeRand(maxVars, sparsity);

      //filter out contradictions and tautologies
      if (newTerm.isContradiction() || newTerm.isTautology()) {
         continue;
      }
      
      //find the location for the new term
      for (uint i = 0; i < _terms.size(); ++i) {
         if (newTerm.implies(_terms[i])) { //new term implies term i, so it can replace it
            _terms[i] = newTerm;

            //remove redundant terms
            ++i;
            while (i < _terms.size()) {
               if (newTerm.implies(_terms[i])) { //new term implies term i, so it can be removed
                  //move back term over term i
                  _terms[i] = _terms.back();
                  _terms.pop_back();

                  if (i == _terms.size()) { [[unlikely]]; //break out of for loop and skip pushing the new node (since it has already overwritten a redundant term)
                     goto AFTER_FINALLY;
                  }
                  //skip increment
                  continue;
               }
               ++i;
            }
         }
      }
      // finally {
      _terms.push_back(newTerm);
      // }
      AFTER_FINALLY:
   }
}

//!remove redundant terms
inform::SopExpr& inform::SopExpr::normalize() { //!TODO: maybe make this a private method
   for (uint i = 1; i < _terms.size(); ++i) {
      for (uint j = 0; j < i;) {
         if (_terms[j].implies(_terms[i])) {
            //move back term over term i
            _terms[i] = _terms.back();
            _terms.pop_back();
            if (i == _terms.size()) { [[unlikely]];
               return self;
            }
            //ensure that the new term i (former back item) is checked
            j = 0;
            continue;
         }
         ++j;
      }
   }
   return self;
}

inform::SopExpr& inform::SopExpr::operator|=(const ProdTerm& term) {
   _terms.push_back(term);
   return self;
   //!TODO: normalize as each term is pushed (will be asymptotically faster than calling normalize() after)
}
inform::SopExpr& inform::SopExpr::operator|=(const SopExpr& other) {
   for (const auto& term : other._terms) {
      _terms.push_back(term);
   }
   return self;
   //!TODO: normalize as each term is pushed (will be asymptotically faster than calling normalize() after)
}
inform::SopExpr& inform::SopExpr::operator&=(const ProdTerm& term) {
   if (!_terms.size()) {
      _terms.push_back(term);
      return self;
   }
   
   for (uint i = 0; i < _terms.size();) {
      _terms[i] &= term;
      if (_terms[i].isContradiction()) {
         //move back term over term i
         _terms[i] = _terms.back();
         _terms.pop_back();
         //ensure the new term i (former back item) is checked
         continue;
      }
      ++i;
   }
   return normalize();
}
inform::SopExpr& inform::SopExpr::operator&=(const SopExpr& other) {
   if (other._terms.size() == 1) {
      return self &= other._terms[0];
   }
   auto tmp = self & other;
   std::destroy_at(this);
   return *new (this) SopExpr(std::move(tmp));
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
inform::SopExpr inform::SopExpr::operator&(const SopExpr& other) { //!TODO: maybe make this a private constructor to avoid the move
   if (!_terms.size()) {
      return other.copy();
   }
   if (!other._terms.size()) { [[unlikely]];
      return copy();
   }
   
   return SopExpr(self, other);
}
//private AND constructor
inform::SopExpr::SopExpr(const SopExpr& lhs, const SopExpr& rhs) {
   _terms.reserve(lhs._terms.size() * rhs._terms.size());
   //(A+B)(C+D) = AC + AD + BC + BD
   for (const auto& i : lhs._terms) {
      for (const auto& j : rhs._terms) {
         //add the product of each unordered pair of terms with one element from each SopExpr
         const ProdTerm tmp = i & j;
         if (!tmp.isContradiction()) { [[likely]]; //push term if it is not a contradiction
            _terms.push_back(tmp);
         }
      }
   }
   normalize();
}



uint mcsl::writef(File& file, const inform::SopExpr& obj, char mode, FmtArgs fmt) {
   switch (mode | CASE_BIT) {
      case 'i': [[fallthrough]];
      case 'u': [[fallthrough]];
      case 'r': [[fallthrough]];
      case 'b': [[fallthrough]];

      case 'c': mcsl::__throw(ErrCode::FS_ERR, FMT("invalid format code (%%%c) for type"), mode);
      case 's': break;
   }

   uint charsPrinted = 0;
   const inform::ProdTerm* it = obj.begin();
   const inform::ProdTerm* end = obj.end();

   if (!it || it >= end) {
      return charsPrinted;
   }

   charsPrinted += file.printf(FMT("(%s)"), *it);
   while (++it < end) {
      charsPrinted += file.printf(FMT(" ⋁ (%s)"), *it);
   }

   return charsPrinted;
}

#endif
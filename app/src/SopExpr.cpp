#ifndef SOP_EXPR_CPP
#define SOP_EXPR_CPP

#include "SopExpr.hpp"

#include "io.hpp"

//!generate a random sum-of-products expression
inform::SopExpr::SopExpr(uint termCount, uint maxVars, uint sparsity) {
   while (_terms.size() < termCount) {
      const ProdTerm newTerm = ProdTerm::makeRand(maxVars, sparsity);

      //filter out contradictions and tautologies
      if (newTerm.isContradiction() || newTerm.isTautology()) {
         continue;
      }
      
      //find the location for the new term
      normalizedPushBack(newTerm);
   }

   // normalize();
   if (isTautology() || isContradiction()) { //restart if a tautology or contradiction is generated
      std::destroy_at(this);
      new (this) SopExpr(termCount, maxVars, sparsity);
   }
}

void inform::SopExpr::pop(uint index) {
   assume(index < _terms.size());
   ProdTerm tmp = _terms.pop_back();
   if (index < _terms.size()) {
      _terms[index] = tmp;
   }
}
void inform::SopExpr::swap(uint lhs, uint rhs) {
   assume(lhs < _terms.size());
   assume(rhs < _terms.size());
   ProdTerm tmp = _terms[lhs];
   _terms[lhs] = _terms[rhs];
   _terms[rhs] = tmp;
}

//!NOTE: discards _terms[index]
uint inform::SopExpr::normalizedPush(ProdTerm term, uint index) {
   assume(index < _terms.size());
   _terms[index] = term;
   
   uint maskPopcount = std::popcount(term.mask());
   bool poppedTerm = false;
   for (uint j = 0; j < index; ++j) {
      ProdTerm termJ = _terms[j];
      // uint maskPopcountJ = std::popcount(termJ.mask());

      // const uint maskDiffMask = term.mask() ^ termJ.mask();
      // const uint conflictMask = ((term.trueMask() & termJ.falseMask()) | (term.falseMask() & termJ.trueMask())) & (term.mask() | termJ.mask());
      
      // const uint maskDiffPopcount = std::popcount(maskDiffMask);
      // const uint conflictPopcount = std::popcount(conflictMask);

      if (termJ.subsumes(term)) {
         pop(index);
         --index;
         break;
      }
   }
   return index + 1;
}
uint inform::SopExpr::normalizedPushBack(ProdTerm term) {
   uint index = _terms.size();
   _terms.push_back(ProdTerm::makeContradiction());
   return normalizedPush(term, index);
}

//!remove redundant terms
inform::SopExpr& inform::SopExpr::normalize() { //!TODO: maybe make this a private method
   for (uint i = 1; i < _terms.size();) {
      ProdTerm termI = _terms[i];
      i = normalizedPush(termI, i);
   }
   
   if (!_terms.size()) {
      _terms.push_back(ProdTerm::makeContradiction());
   }
   return self;
}

inform::SopExpr& inform::SopExpr::operator|=(const ProdTerm& term) {
   normalizedPushBack(term);
   return self;
}
inform::SopExpr& inform::SopExpr::operator|=(const SopExpr& other) {
   for (const auto& term : other._terms) {
      normalizedPushBack(term);
   }
   return self;
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
         pop(i);
         //ensure the new term i (former back item) is checked
         continue;
      }
      ++i;
   }
   if (!_terms.size()) {
      _terms.push_back(ProdTerm::makeContradiction());
   } else {
      normalize();
   }
   return self;
}
inform::SopExpr& inform::SopExpr::operator&=(const SopExpr& other) {
   // if (other._terms.size() == 1) {
   //    return self &= other._terms[0];
   // }
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
      if (i.isContradiction()) {
         continue;
      }
      for (const auto& j : rhs._terms) {
         if (j.isContradiction()) {
            continue;
         }
         //add the product of each unordered pair of terms with one element from each SopExpr
         const ProdTerm tmp = i & j;
         if (!tmp.isContradiction()) { [[likely]]; //push term if it is not a contradiction
            normalizedPushBack(tmp);
         }
      }
   }
   if (!_terms.size()) {
      _terms.push_back(ProdTerm::makeContradiction());
   } else {
      normalize();
      // if (!_terms.size()) {
      //    _terms.push_back(ProdTerm::makeContradiction());
      // }
   }
}

bool inform::SopExpr::isContradiction() const {
   return _terms.size() == 1 && _terms[0].isContradiction();
}
bool inform::SopExpr::isTautology() const {
   return _terms.size() == 1 && _terms[0].isTautology();
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
      charsPrinted += file.printf(FMT(" %s (%s)"), inform::OR, *it);
   }

   return charsPrinted;
}

#endif
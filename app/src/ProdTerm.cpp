#ifndef PROD_TERM_CPP
#define PROD_TERM_CPP

#include "ProdTerm.hpp"

#include "rand.hpp"
#include "pair.hpp"
#include "assert.hpp"
#include "io.hpp"

inform::ProdTerm inform::ProdTerm::makeRand() {
   mcsl::pair<uint32, uint32> data = std::bit_cast<mcsl::pair<uint32,uint32>>(mcsl::rand() & mcsl::rand() & mcsl::rand() & mcsl::rand());
   if (data.second) { [[likely]];
      return make(data.first, data.second);
   }
   [[unlikely]];
   return makeRand();
}
inform::ProdTerm inform::ProdTerm::makeRand(uint maxVars, uint sparsity) {
   mcsl::pair<uint32, uint32> data = std::bit_cast<mcsl::pair<uint32,uint32>>(mcsl::rand());
   data.second &= ((1 << maxVars) - 1);
   for (uint i = 0; i < sparsity; ++i) {
      data.second &= mcsl::rand();
   }
   if (data.second) { [[likely]];
      return make(data.first, data.second);
   }
   [[unlikely]];
   return makeRand(maxVars, sparsity);
}

inform::ProdTerm::Status inform::ProdTerm::operator[](ubyte i) const {
   assume(i < (8 * sizeof(_mask)));
   if (!((_mask >> i) & 1)) {
      return Status::NULL;
   }
   return (_vals >> i) & 1 ? TRUE : FALSE;
}

inform::ProdTerm& inform::ProdTerm::operator&=(const ProdTerm& other) {
   uint overlapMask = _mask & other._mask;
   if (overlapMask) { //if theres overlap
      if ((_vals & overlapMask) != (other._vals & overlapMask)) { //overlap must be the same
         //set to contradiction
         _mask = 0;
         _vals = -1;
         return self;
      }
   }
   // (ulong&)self |= std::bit_cast<unsigned long>(other);
   _vals |= other._vals;
   _mask |= other._mask;
   return self;
}

bool inform::ProdTerm::isContradiction() const {
   return !_mask && _vals;
}
bool inform::ProdTerm::isTautology() const {
   return !_mask && !_vals;
}

bool inform::ProdTerm::operator==(const ProdTerm& other) const {
   return _mask == other._mask && _vals == other._vals;
}

bool inform::ProdTerm::implies(const ProdTerm& other) const {
   return ((other._mask & _mask) == other._mask) && (_vals & other._mask) == (other._vals & other._mask);
}


uint mcsl::writef(File& file, const inform::ProdTerm& obj, char mode, FmtArgs fmt) {
   switch (mode | CASE_BIT) {
      case 'i': [[fallthrough]];
      case 'u': [[fallthrough]];
      case 'r': [[fallthrough]];
      case 'b': return writef(file, obj.toInt(), mode, fmt);

      case 'c': mcsl::__throw(ErrCode::FS_ERR, FMT("invalid format code (%%%c) for type"), mode);
      case 's': break;
   }

   if (obj.isContradiction()) {
      return writef(file, FMT("⊥"), mode, fmt);
   }
   if (obj.isTautology()) {
      return writef(file, FMT("⊤"), mode, fmt);
   }

   //does not respect width parameters
   uint charsPrinted = 0;
   bool firstTerm = true;
   for (ubyte i = 0; i < 4 * sizeof(inform::ProdTerm); ++i) {
      inform::ProdTerm::Status status = obj[i];
      if (status == inform::ProdTerm::NULL) {
         continue;
      }

      if (firstTerm) {
         firstTerm = false;
      } else {
         charsPrinted += file.printf(FMT(" ⋀ "));
      }

      if (status == inform::ProdTerm::FALSE) {
         charsPrinted += file.printf(FMT("¬"));
      }
      charsPrinted += file.printf(FMT("x%u"), i);
   }
   return charsPrinted;
}


#endif
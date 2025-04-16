#ifndef PROD_TERM_CPP
#define PROD_TERM_CPP

#include "ProdTerm.hpp"

#include "rand.hpp"
#include "pair.hpp"
#include "assert.hpp"

inform::ProdTerm inform::ProdTerm::makeRand() {
   mcsl::pair<uint32, uint32> data = std::bit_cast<mcsl::pair<uint32,uint32>>(mcsl::rand() & mcsl::rand());
   if (data.second) { [[likely]];
      return ProdTerm{data.first, data.second};
   }
   [[unlikely]];
   return makeRand();
}

inform::ProdTerm::Status inform::ProdTerm::operator[](ubyte i) {
   assume(i < (8 * sizeof(_varsMask)));
   if (!((_varsMask >> i) & 1)) {
      return Status::NULL;
   }
   return (_varVals >> i) & 1 ? TRUE : FALSE;
}

inform::ProdTerm& inform::ProdTerm::operator&=(const ProdTerm& other) {
   if (_varsMask & other._varsMask) { //if theres overlap
      if (_varVals & _varsMask != other._varVals & other._varsMask) { //overlap must be the same
         _varsMask = 0;
         _varVals = 0;
         return self;
      }
   }
   // (ulong&)self |= std::bit_cast<unsigned long>(other);
   _varVals |= other._varVals;
   _varsMask |= other._varsMask;
}

#endif
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
   assume(i < (8 * sizeof(_mask)));
   if (!((_mask >> i) & 1)) {
      return Status::NULL;
   }
   return (_vals >> i) & 1 ? TRUE : FALSE;
}

inform::ProdTerm& inform::ProdTerm::operator&=(const ProdTerm& other) {
   uint overlapMask = _mask & other._mask;
   if (overlapMask) { //if theres overlap
      if (_vals & overlapMask != other._vals & overlapMask) { //overlap must be the same
         //set to contradiction
         _mask = 0;
         _vals = 0;
         return self;
      }
   }
   // (ulong&)self |= std::bit_cast<unsigned long>(other);
   _vals |= other._vals;
   _mask |= other._mask;
   return self;
}

bool inform::ProdTerm::operator==(const ProdTerm& other) const {
   return _mask == other._mask && _vals == other._vals;
}

#endif
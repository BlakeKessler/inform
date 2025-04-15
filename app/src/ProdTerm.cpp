#ifndef PROD_TERM_CPP
#define PROD_TERM_CPP

#include "ProdTerm.hpp"

#include "rand.hpp"
#include "pair.hpp"

inform::ProdTerm inform::ProdTerm::makeRand() {
   mcsl::pair<uint32, uint32> data = std::bit_cast<mcsl::pair<uint32,uint32>>(mcsl::rand() & mcsl::rand());
   data.first &= data.second; //ensure canonical representation (all masked-out terms are 0)
   return ProdTerm{data.first, data.second};
}

#endif
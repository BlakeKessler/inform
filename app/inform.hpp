#ifndef INFORM_HPP
#define INFORM_HPP

#include "MCSL.hpp"
#include "str_slice.hpp"

namespace inform {
   struct ProdTerm;
   class SopExpr;

   class PremiseSet;

   extern mcsl::str_slice NOT;
   extern mcsl::str_slice OR;
   extern mcsl::str_slice AND;
   extern mcsl::str_slice IMPLIES;
};

#endif
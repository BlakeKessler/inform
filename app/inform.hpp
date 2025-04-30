#ifndef INFORM_HPP
#define INFORM_HPP

#include "MCSL.hpp"
#include "str_slice.hpp"

namespace inform {
   //the classes used to represent formal logic entities
   //see ProdTerm.hpp, SopExpr.hpp, or PremiseSet.hpp for further details
   struct ProdTerm;
   class SopExpr;
   class PremiseSet;

   //the strings that are printed indicated connectives
   //can be changed at runtime to facilitate multiple encoding options
   extern mcsl::str_slice NOT;
   extern mcsl::str_slice OR;
   extern mcsl::str_slice AND;
   extern mcsl::str_slice IMPLIES;
   extern mcsl::str_slice CONTRADICTION;
   extern mcsl::str_slice TAUTOLOGY;
};

#endif
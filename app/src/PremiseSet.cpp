#ifndef PREMISE_SET_CPP
#define PREMISE_SET_CPP

#include "PremiseSet.hpp"

inform::PremiseSet::PremiseSet(const mcsl::arr_span<SopExpr> exprs):
   _exprs(exprs) {
      //make conclusion
      if (_exprs.size()) {
         _conclusion = _exprs[0];
         for (uint i = 1; i < _exprs.size(); ++i) {
            _conclusion &= _exprs[i];
         }
         _conclusion.normalize();
      }
}

#endif
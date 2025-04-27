#ifndef PREMISE_SET_CPP
#define PREMISE_SET_CPP

#include "PremiseSet.hpp"

#include "io.hpp"

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
inform::PremiseSet::PremiseSet(PremiseSet&& other):
   _exprs(std::move(other._exprs)), _conclusion(std::move(other._conclusion)) {
      
}
inform::PremiseSet::PremiseSet(const PremiseSet& other):
   _exprs(other._exprs), _conclusion(other._conclusion) {
      
}

inform::PremiseSet::PremiseSet(uint premsPerProof, uint termCount, uint maxVars, uint sparsity):
   _exprs(premsPerProof),_conclusion() {
      for (uint i = 0; i < premsPerProof; ++i) {
         _conclusion &= *new (_exprs + i) SopExpr(termCount, maxVars, sparsity);
      }
}



uint mcsl::writef(File& file, const inform::PremiseSet& obj, char mode, FmtArgs fmt) {
   switch (mode | CASE_BIT) {
      case 'i': [[fallthrough]];
      case 'u': [[fallthrough]];
      case 'r': [[fallthrough]];
      case 'b': [[fallthrough]];

      case 'c': mcsl::__throw(ErrCode::FS_ERR, FMT("invalid format code (%%%c) for type"), mode);
      case 's': break;
   }

   uint charsPrinted = 0;
   const inform::SopExpr* it = obj.begin();
   const inform::SopExpr* end = obj.end();

   if (!it) {
      return charsPrinted;
   }
   for (; it < end; ++it) {
      charsPrinted += file.printf(FMT("%s\n"), *it);
   }

   charsPrinted += file.printf(FMT("⇒ %s\n"), obj.conclusion());

   return charsPrinted;
}

#endif
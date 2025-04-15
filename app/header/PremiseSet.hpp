#ifndef PREMISE_SET_HPP
#define PREMISE_SET_HPP

#include "inform.hpp"
#include "SopExpr.hpp"

#include "array.hpp"

class inform::PremiseSet {
   private:
      mcsl::array<SopExpr> _exprs;
      SopExpr _conclusion;

   public:
      PremiseSet(const mcsl::arr_span<SopExpr> exprs);

      const mcsl::arr_span<SopExpr> exprs() const;
      const SopExpr conclusion() const;
      const SopExpr randConcSubset() const;
};

#endif
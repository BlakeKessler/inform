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
      PremiseSet(PremiseSet&&);
      PremiseSet(const PremiseSet&);
      PremiseSet&& move() { return std::move(self); }
      PremiseSet copy() const { return self; }
      static PremiseSet makeRand(uint premsPerProof, uint termCount);

      PremiseSet& operator=(PremiseSet&& other) = default;
      PremiseSet& operator=(const PremiseSet& other) = default;
      
      const mcsl::arr_span<SopExpr> exprs() const { return _exprs.span(); }
      const SopExpr conclusion() const { return _conclusion; }
      const SopExpr randConcSubset() const;

      SopExpr* begin() { return _exprs.begin(); }
      SopExpr* end() { return _exprs.end(); }
      const SopExpr* begin() const { return _exprs.begin(); }
      const SopExpr* end() const { return _exprs.end(); }
};

namespace mcsl {
   uint writef(File&, const inform::PremiseSet&, char, FmtArgs);
};

#endif
// A class representing a set of premises, where each premise is a sum-of-products expression.
// Objects have an array of sum-of-product premise expressions, and an additional sum-of-products expression for the strongest possible conclusion of the premises
// This can be used to represent any possible proof's premises and conclusion (see ProdTerm.hpp if support for more variables is needed)

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
      
      //!randomly generate a PremiseSet object
      //!had to be a constructor instead of a static method because of memory management
      //!for explanations of the parameters, see the help menu (run the executable with no parameters)
      PremiseSet(uint premsPerProof, uint termCount, uint maxVars, uint sparsity, bool allowContConc);

      PremiseSet move() { return std::move(self); }
      PremiseSet copy() const { return self; }

      PremiseSet& operator=(PremiseSet&& other) = default;
      PremiseSet& operator=(const PremiseSet& other) = default;
      
      const mcsl::arr_span<SopExpr> exprs() const { return _exprs.span(); }
      const SopExpr& conclusion() const { return _conclusion; }
      // const SopExpr randConcSubset() const;

      SopExpr* begin() { return _exprs.begin(); }
      SopExpr* end() { return _exprs.end(); }
      const SopExpr* begin() const { return _exprs.begin(); }
      const SopExpr* end() const { return _exprs.end(); }
};

namespace mcsl {
   uint writef(File&, const inform::PremiseSet&, char, FmtArgs);
};

#endif
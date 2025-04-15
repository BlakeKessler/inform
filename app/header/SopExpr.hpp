#ifndef INFORM_SOP_EXPR_HPP
#define INFORM_SOP_EXPR_HPP

#include "inform.hpp"

#include "ProdTerm.hpp"

#include "dyn_arr.hpp"
#include "arr_span.hpp"

//!Sum-of-products Expression
class inform::SopExpr : public mcsl::contig_base<ProdTerm> {
   private:
      //!TODO: maybe add a _baseTerm? maybe make a tree-like structure?
      mcsl::dyn_arr<ProdTerm> _terms;

   public:
      SopExpr():_terms{} {}
      SopExpr(mcsl::arr_span<ProdTerm> terms):_terms{terms} {}
      SopExpr copy() const;
      static SopExpr makeRand();

      SopExpr& normalize();

      SopExpr& operator&=(const ProdTerm& other);
      SopExpr& operator&=(const SopExpr& other);

      SopExpr& operator|=(const ProdTerm& other);
      SopExpr& operator|=(const SopExpr& other);

      #pragma region contig
      [[gnu::pure]] constexpr uint size() const { return _terms.size(); }
      [[gnu::pure]] constexpr uint capacity() const { return _terms.capacity(); }

      [[gnu::pure]] constexpr ProdTerm* const* ptr_to_buf() { return _terms.ptr_to_buf(); }
      [[gnu::pure]] constexpr ProdTerm* data() { return _terms.data(); }
      [[gnu::pure]] constexpr ProdTerm* begin() { return _terms.begin(); }
      [[gnu::pure]] constexpr const ProdTerm* const* ptr_to_buf() const { return _terms.ptr_to_buf(); }
      [[gnu::pure]] constexpr const ProdTerm* data() const { return _terms.data(); }
      [[gnu::pure]] constexpr const ProdTerm* begin() const { return _terms.begin(); }

      bool reserve(const uint newSize) { return _terms.reserve(newSize); }
      bool reserve_exact(const uint newSize) { return _terms.reserve_exact(newSize); }
      ProdTerm* release() { return _terms.release(); }
      ProdTerm* push_back(ProdTerm&& obj) { return _terms.push_back(obj); }
      ProdTerm* push_back(const ProdTerm& obj) { return _terms.push_back(obj); }
      ProdTerm pop_back() { return _terms.pop_back(); }
      ProdTerm* emplace(const uint i, auto&&... args) requires mcsl::valid_ctor<ProdTerm, decltype(args)...> { return _terms.emplace(i, args...); }
      ProdTerm* emplace_back(auto&&... args) requires mcsl::valid_ctor<ProdTerm, decltype(args)...> { return _terms.emplace_back(args...); }
      #pragma endregion contig
};

#endif
#ifndef MAIN_CPP
#define MAIN_CPP

#include "PremiseSet.hpp"

#include "str_to_num.hpp"
#include "dyn_arr.hpp"
#include "io.hpp"

int main(int argc, char** argv) {
   if (argc < 6) {
      mcsl::err_printf(mcsl::FMT("must pass 5 arguments\n"));
      return EXIT_FAILURE;
   }

   uint proofsToGen = mcsl::str_to_uint(argv[1], std::strlen(argv[1]));
   uint premsPerProof = mcsl::str_to_uint(argv[2], std::strlen(argv[2]));
   uint termCount = mcsl::str_to_uint(argv[3], std::strlen(argv[3]));
   uint maxVars = mcsl::str_to_uint(argv[4], std::strlen(argv[4]));
   uint sparsity = mcsl::str_to_uint(argv[5], std::strlen(argv[5]));

   for (uint i = 0; i < proofsToGen; ++i) {
      // inform::PremiseSet(premsPerProof, termCount, maxVars, sparsity);
      mcsl::printf(mcsl::FMT("============\n%s============\n"), inform::PremiseSet(premsPerProof, termCount, maxVars, sparsity));
   }
}

#endif
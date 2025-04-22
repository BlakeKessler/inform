#ifndef MAIN_CPP
#define MAIN_CPP

#include "PremiseSet.hpp"

#include "str_to_num.hpp"
#include "dyn_arr.hpp"
#include "io.hpp"

int main(int argc, char** argv) {
   if (argc < 4) {
      mcsl::err_printf(mcsl::FMT("must pass 3 arguments\n"));
      return EXIT_FAILURE;
   }

   uint proofsToGen = mcsl::str_to_uint(argv[1], std::strlen(argv[1]));
   uint premsPerProof = mcsl::str_to_uint(argv[2], std::strlen(argv[2]));
   uint termCount = mcsl::str_to_uint(argv[3], std::strlen(argv[3]));

   for (uint i = 0; i < proofsToGen; ++i) {
      inform::PremiseSet::makeRand(premsPerProof, termCount);
      // mcsl::printf(mcsl::FMT("============\n%s============\n"), inform::PremiseSet::makeRand(premsPerProof, termCount));
   }
}

#endif
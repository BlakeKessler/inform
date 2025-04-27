#ifndef MAIN_CPP
#define MAIN_CPP

#include "PremiseSet.hpp"

#include "str_to_num.hpp"
#include "dyn_arr.hpp"
#include "io.hpp"
#include "rand.hpp"

#ifndef NDEBUG
   #include <ctime>
#endif

int main(int argc, char** argv) {
   #ifndef NDEBUG
   mcsl::srand(std::time(nullptr));
   #endif

   //parameter default values
   uint proofsToGen = 1; //-n=[0-9]+
   uint premsPerProof = 2; //-p=[0-9]+
   uint termCount = 2; //-t=[0-9]+
   uint maxVars = 2; //-v=[0-9]+
   uint sparsity = 0; //-s=[0-9]+
   bool allowContConc = false; //-C
   mcsl::str_slice outPath{}; //-o=[^ ]+
   mcsl::str_slice inPath{}; //[^-/ ][^ ]+

   //parse inputs
   bool badParam = false;
   for (int i = 1; i < argc; ++i) {
      mcsl::str_slice arg = FMT(argv[i]);
      if (arg.size() < 2 || arg[0] != '-') {
         goto BAD_FLAG;
      }
      
      if (arg.size() == 2 && arg[1] == 'C') {
         allowContConc = true;
         continue;
      }
      if (arg.size() < 4) {
         goto BAD_FLAG;
      }
      if (arg[2] == '=') {
         mcsl::str_slice valStr = arg.slice(3, arg.size()-3);
         switch (arg[1]) {
            case 'n':   proofsToGen = mcsl::str_to_uint(valStr.begin(), valStr.size()); break;
            case 'p': premsPerProof = mcsl::str_to_uint(valStr.begin(), valStr.size()); break;
            case 't':     termCount = mcsl::str_to_uint(valStr.begin(), valStr.size()); break;
            case 'v':       maxVars = mcsl::str_to_uint(valStr.begin(), valStr.size()); break;
            case 's':      sparsity = mcsl::str_to_uint(valStr.begin(), valStr.size()); break;
            case 'o':       outPath = valStr; break;

            default: goto BAD_FLAG;
         }
         continue;
      }

      BAD_FLAG:
      if (arg.size() && arg[0] != '-') { //input batch file
         inPath = arg;
      } else {
         badParam = true;
         mcsl::err_printf(FMT("\033[1;31mERROR:\033[22;39m unrecognized flag: %s\n"), arg);
      }
   }
   //validate paramters
   if (maxVars > (8 * sizeof(uint))) {
      badParam = true;
      mcsl::err_printf(FMT("\033[1;31mERROR:\033[22;39m variable limit exceeded (%i > %i)\n"), maxVars, 8 * sizeof(uint));
   }

   //abort if there are invalid parameters
   if (inPath.size()) {
      mcsl::err_printf(FMT("\033[1;31mERROR:\033[22;39m batch input files are not yet supported\n"));
      badParam = true;
   }
   if (badParam) {
      mcsl::err_printf(FMT("\033[1;31mABORTING\033[22;39m\n"));
      return EXIT_FAILURE;
   }

   //open output file
   mcsl::File* out;
   if (outPath.size()) {
      out = new mcsl::File(mcsl::Path(outPath), "w");
   } else {
      out = &mcsl::stdout;
   }

   //generate the proofs
   for (uint i = 0; i < proofsToGen; ++i) {
      auto tmp = inform::PremiseSet(premsPerProof, termCount, maxVars, sparsity, allowContConc);
      out->printf(mcsl::FMT("============\n%s"), tmp);
   }
   mcsl::printf(mcsl::FMT("============\n"));

   if (outPath.size()) {
      delete out;
   }
   return EXIT_SUCCESS;
}

#endif
#ifndef MAIN_CPP
#define MAIN_CPP

#include "inform.hpp"
#include "PremiseSet.hpp"

#include "str_to_num.hpp"
#include "dyn_arr.hpp"
#include "io.hpp"
#include "rand.hpp"

#ifndef NDEBUG
   #include <ctime>
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

//generate the proofs
void genProofs(mcsl::File& out) {
   for (uint i = 0; i < proofsToGen; ++i) {
      auto tmp = inform::PremiseSet(premsPerProof, termCount, maxVars, sparsity, allowContConc);
      out.printf(mcsl::FMT("============\n%s"), tmp);
   }
   out.printf(mcsl::FMT("============\n"));
}

//parse inputs
bool parseInputs(int argc, char** argv) {
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
      if (arg.size() == 2 && arg[1] == 'A') {
         inform::NOT = mcsl::FMT("~");
         inform::OR = mcsl::FMT("|");
         inform::AND = mcsl::FMT("&");
         inform::IMPLIES = mcsl::FMT("$");
         inform::CONTRADICTION = mcsl::FMT("^");
         inform::TAUTOLOGY = mcsl::FMT("T");
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
   return badParam;
}

//validate parameters
bool checkParams() {
   bool badParam = false;
   if (maxVars > (8 * sizeof(uint))) {
      badParam = true;
      mcsl::err_printf(FMT("\033[1;31mERROR:\033[22;39m variable limit exceeded (%i > %i)\n"), maxVars, 8 * sizeof(uint));
   }
   // if (termCount > maxVars) {
   //    badParam = true;
   //    mcsl::err_printf(FMT("\033[1;31mERROR:\033[22;39m may not have more terms than variables (%i > %i)\n"), termCount, maxVars);
   // }
   if (inPath.size()) {
      mcsl::err_printf(FMT("\033[1;31mERROR:\033[22;39m batch input files are not yet supported\n"));
      badParam = true;
   }
   return badParam;
}

//print help page
void help(mcsl::str_slice exePath) {
   mcsl::printf(FMT("\033[1mFLAGS:\033[22m\n"));

   mcsl::printf(FMT("\033[4m-n\033[24m: the number of proofs to generate\n"));
   mcsl::printf(FMT("\033[4m-p\033[24m: the number of premises in the proofs\n"));
   mcsl::printf(FMT("\033[4m-t\033[24m: the number of terms in each premise\n"));
   mcsl::printf(FMT("\033[4m-v\033[24m: the number of variables\n"));
   mcsl::printf(FMT("\033[4m-s\033[24m: the chance to skip an individual step in simplification\n"));
   mcsl::printf(FMT("\033[4m-C\033[24m: prevents generation of proofs of the contradiction literal when present\n"));
   mcsl::printf(FMT("\033[4m-o\033[24m: the path to the output file (defaults to stdout)\n"));
   mcsl::printf(FMT("\033[4m-A\033[24m: print output with ASCII encoding instead of using unicode characters when present\n"));
   mcsl::printf(FMT("\033[4m-h\033[24m or \033[4m--help\033[24m: print this menu if it is the only flag, throws an error otherwise\n"));

   mcsl::printf(FMT("\n\033[1mEXAMPLE:\033[22m \033[4m%s -n=1 -p=2 -t=3 -v=4 -s=5 -C -o=RESULTS.txt\033[24m\n"), exePath);
   mcsl::printf(FMT("generates 1 proof that will not have contradiction as a conclusion, with 2 premises, each with 3 terms, the set of variables used in the proof being {x0, x1, x2, x3}, having a 5%% chance to undersimplify at each step, and prints the results to the file \'RESULTS.txt\' (overwriting the previous contents of that file if it already exists)\n"));
   mcsl::printf(FMT("\033[1mEXAMPLE:\033[22m \033[4m%s -n=1 -p=2 -t=2 -v=2 -s=0\033[24m\n"), exePath);
   mcsl::printf(FMT("the default parameters\n"));

   mcsl::printf(FMT("\nAny parameters not specified when running the program will use their default values.\nFlags may be passed in any order.\n"));
}

//main
int main(int argc, char** argv) {
   #ifdef NDEBUG
   mcsl::srand(std::time(nullptr));
   #endif

   //parse inputs
   if (argc == 1 || (argc == 2 && (FMT(argv[1]) == FMT("-h") || FMT(argv[1]) == FMT("--help")))) {
      help(FMT(argv[0]));
      return EXIT_SUCCESS;
   }
   bool badParam = parseInputs(argc, argv);
   badParam |= checkParams();

   //abort if there are invalid parameters
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

   //generate proofs
   genProofs(*out);

   //close output file
   if (outPath.size()) {
      delete out;
   }

   return EXIT_SUCCESS;
}

#endif
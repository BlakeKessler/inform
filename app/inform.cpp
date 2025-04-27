#ifndef INFORM_CPP
#define INFORM_CPP

#include "inform.hpp"

mcsl::str_slice inform::NOT = mcsl::FMT("¬");
mcsl::str_slice inform::OR = mcsl::FMT("⋁");
mcsl::str_slice inform::AND = mcsl::FMT("⋀");
mcsl::str_slice inform::IMPLIES = mcsl::FMT("→");
mcsl::str_slice inform::CONTRADICTION = mcsl::FMT("⊥");
mcsl::str_slice inform::TAUTOLOGY = mcsl::FMT("⊤");

#endif
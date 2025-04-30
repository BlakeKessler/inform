# Inform

A program for generating first order logic practice problems

It internally represents everything as sum-of-products/disjunctive normal form expressions. It generates random product terms, combines those into sum-of-products expressions, combines those to create the premises of proofs, and ANDs them together to create the conclusion.

It is currently significantly less complete than I would like it to be. It does not simplify expressions nearly enough, and can only print in disjunctive normal forms. If you want to fix that, an off-the-shelf FOL expression simplifier program would probably work reasonably well. A regex-based program would probably be relatively easy to create (the formatting of the output is very simple, and always has exactly one layer of nesting). However, the most efficient way to fix the simplifier would be expanding on the inform::SopExpr::normalizedPush method (if you can stand learning how someone else's codebase works).

When you clone the repo, make sure that the `mcsl` module gets cloned as well, and that it is on the `cpp` branch. If this directory does not contain an `mcsl` directory, run the command `git clone https://github.com/BlakeKessler/mcsl.git && cd mcsl && git checkout cpp` to clone the correct branch.

To build the project from source, navigate to this directory in a terminal and run the command `make -s setup && make -s release` for a release build and `make -s setup && make -s debug` for a debug build (basically the same as the release build, but always uses the same initial seed for RNG). The project appears to compile properly with clang-18 and clang-19, but not with gcc (some small changes to the mcsl would probably fix that, but they would make the code less readable) or MSVC. The release build will create an executable `_build/out/inform`, and the debug build will create an executable `_build/out/inform.out`.

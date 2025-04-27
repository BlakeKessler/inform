#directories and files
MODULES := mcsl app

ALL_CODE_DIRS := $(foreach dir, $(MODULES),${shell find -L $(dir) -type d -print})
ALL_SRC_FILES := $(foreach dir, $(ALL_CODE_DIRS), $(wildcard $(dir)/*.cpp))
ALL_HEADER_FILES := $(foreach dir, $(ALL_CODE_DIRS), $(wildcard $(dir)/*.hpp))

ALL_OBJ_FILES := $(ALL_SRC_FILES:%.cpp=_build/%.o)
# ALL_PCH_FILES := $(ALL_HEADER_FILES:%=_build/%.pch)

#compiler
COMPILER := clang++ -std=c++23
FLAGS := -g -Wall -Wextra -pedantic -pedantic-errors -ftemplate-backtrace-limit=4 -fdiagnostics-show-category=name -Wno-gcc-compat -Wno-trigraphs -Wno-pessimizing-move
# FLAGS := -g -Wall -Wextra -pedantic -pedantic-errors -ftemplate-backtrace-limit=4 -fdiagnostics-show-category=name -Wno-gcc-compat -Wno-trigraphs -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
# COMPILER := g++ -std=c++23
# FLAGS := -g -Wall -Wextra -pedantic -pedantic-errors -ftemplate-backtrace-limit=4 -Wno-trigraphs -Wno-attributes -Wno-parentheses -Wno-class-memaccess -Wno-pessimizing-move

# LINKER_FLAGS := -lbacktrace

#compile commands
COMPILE := $(COMPILER) $(FLAGS) $(addprefix -I, $(MODULES) $(filter %/header, $(ALL_CODE_DIRS)))


#clean
.PHONY: clean
clean:
	rm -rf _build

#set up directory structure
.PHONY: setup
setup:
	mkdir -p $(addprefix _build/,$(ALL_CODE_DIRS))
	mkdir -p _build/out


#compile object files
$(ALL_OBJ_FILES): _build/%.o : %.cpp
	($(COMPILE) -c $^ -o $@) && echo $@


.PHONY: objects
objects: $(ALL_OBJ_FILES)

#compile unit test files
.PHONY: debug
debug: app/main.cpp | $(ALL_OBJ_FILES)
	$(COMPILE) $(shell find _build | grep "\.o$$") -o _build/out/inform.out
.PHONY: release
release: app/main.cpp | $(ALL_OBJ_FILES)
	$(COMPILE) -DNDEBUG $(shell find _build | grep "\.o$$") -o _build/out/inform
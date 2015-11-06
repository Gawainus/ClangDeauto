CLANG_LEVEL := ../../..

TOOLNAME = clang-deauto

# No plugins, optimize startup time.
TOOL_NO_EXPORTS = 1

include $(CLANG_LEVEL)/../../Makefile.config
LINK_COMPONENTS := $(TARGETS_TO_BUILD) asmparser bitreader support mc option
USEDLIBS = clangTooling.a clangFrontend.a clangSerialization.a clangDriver.a \
           clangRewriteFrontend.a clangRewrite.a clangParse.a clangSema.a \
           clangAnalysis.a clangAST.a clangASTMatchers.a clangEdit.a \
           clangLex.a clangBasic.a

include $(CLANG_LEVEL)/Makefile

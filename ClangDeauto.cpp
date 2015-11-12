//
// Created by yumen on 11/4/15.
//

// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"

// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Refactoring.h"

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::tooling;
using namespace clang::ast_matchers;

using clang::tooling::CommonOptionsParser;
using clang::tooling::RefactoringTool;
using clang::tooling::Replacement;
using clang::tooling::CompilationDatabase;
using clang::tooling::newFrontendActionFactory;


// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory ClangDeautoCategory("clang-deauto options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nContact Yumen if any questions or bugs.\n");

namespace {
    cl::opt<string> BuildPath(
            cl::Positional,
            cl::desc("<build-path>"));
    cl::list<string> SourcePaths(
            cl::Positional,
            cl::desc("<source0> [... <sourceN>]"),
            cl::OneOrMore);


    class AutoTypeCallback : public MatchFinder::MatchCallback {

    public:
      virtual void run (const MatchFinder::MatchResult &Result) {
        if (const AutoType *AT = Result.Nodes.getNodeAs<AutoType>("autoType")) {
          llvm::outs() << AT->isDeduced() << "\n";
          llvm::outs() << AT->isSugared() << "\n";
          llvm::outs() << AT->getDeducedType().getAsString() << "\n";

        }

      }
    };
}

int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, ClangDeautoCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  AutoTypeCallback autoTypeCB;
  TypeMatcher atm = autoType().bind("autoType");

  MatchFinder Finder;
  Finder.addMatcher(atm, &autoTypeCB);

  return Tool.run(newFrontendActionFactory(&Finder).get());
}

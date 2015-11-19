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



    class DeautoCallback : public MatchFinder::MatchCallback {

    public:
      DeautoCallback(tooling::Replacements * replace) : Replace(replace) {
        m_currType = "Not_Deduced";
      }
      virtual void run (const MatchFinder::MatchResult &Result) {

        if (const ValueDecl *VD = Result.Nodes.getNodeAs<ValueDecl>("valueDecl")) {
          string name = VD->getNameAsString();
          string type = VD->getType().getAsString();
          if (name.compare("main") != 0) {
            llvm::outs() << "Value Decl" << "\n";
            llvm::outs() << "Name: " << name << "\n";
            llvm::outs() << "Type: " << type << "\n";
            m_currType = type;
          }
        }

        if (const TypeLoc *TL = Result.Nodes.getNodeAs<TypeLoc>("typeLoc")) {
          string type = TL->getType().getAsString();
          if (type.compare("auto") == 0) {
            llvm::outs() << type << "\n";
            llvm::outs() << TL->getSourceRange().getBegin().printToString(*Result.SourceManager) << "\n";
            Replace->insert(
                      Replacement(
                        *Result.SourceManager,
                        CharSourceRange::getTokenRange(
                          TL->getSourceRange()
                        ),
                        m_currType
                        )
            );
            llvm::outs() << "\n";
          }
        }
      }

    private:
      string m_currType;
      tooling::Replacements *Replace;
    };

} // end of anonymous namespace

int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, ClangDeautoCategory);

  RefactoringTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  DeautoCallback deautoCB(&Tool.getReplacements());

  TypeMatcher atm = autoType().bind("autoType");
  TypeLocMatcher tlm = typeLoc(isExpansionInMainFile()).bind("typeLoc");
  DeclarationMatcher vm = valueDecl(isExpansionInMainFile()).bind("valueDecl");

  MatchFinder Finder;
  Finder.addMatcher(atm, &deautoCB);
  Finder.addMatcher(tlm, &deautoCB);
  Finder.addMatcher(vm, &deautoCB);

  return Tool.runAndSave(newFrontendActionFactory(&Finder).get());
}

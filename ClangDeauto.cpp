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
static cl::extrahelp MoreHelp("\nContact Yumen for more help.");

int main(int argc, const char **argv) {
  CommonOptionsParser OptionsParser(argc, argv, ClangDeautoCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());
  return Tool.run(newFrontendActionFactory<clang::SyntaxOnlyAction>().get());
}


/*
namespace {
    cl::opt<string> BuildPath(
            cl::Positional,
            cl::desc("<build-path>"));
    cl::list<string> SourcePaths(
            cl::Positional,
            cl::desc("<source0> [... <sourceN>]"),
            cl::OneOrMore);
    cl::opt<string> OriginalMethodName("method", cl::ValueRequired,
                                       cl::desc("Method name to replace"));
    cl::opt<string> ClassName("class", cl::ValueRequired,
                              cl::desc("Name of the class that has this method"),
                              cl::ValueRequired);
    cl::opt<string> NewMethodName("newname",
                                  cl::desc("New method name"),
                                  cl::ValueRequired);


    class ChangeMemberDecl : public ast_matchers::MatchFinder::MatchCallback{
        tooling::Replacements *Replace;
    public:
        ChangeMemberDecl(tooling::Replacements *Replace) : Replace(Replace) {}
        virtual void run(const ast_matchers::MatchFinder::MatchResult &Result) {
            const CXXMethodDecl *method =
                    Result.Nodes.getNodeAs<CXXMethodDecl>("methodDecl");
            Replace->insert(Replacement(
                    *Result.SourceManager,
                    CharSourceRange::getTokenRange(
                            SourceRange(method->getLocation())), NewMethodName));
        }
    };

    class ChangeMemberCall : public ast_matchers::MatchFinder::MatchCallback{
        tooling::Replacements *Replace;
    public:
        ChangeMemberCall(tooling::Replacements *Replace) : Replace(Replace) {}
        virtual void run(const ast_matchers::MatchFinder::MatchResult &Result) {
            const MemberExpr *member =
                    Result.Nodes.getNodeAs<MemberExpr>("member");
            Replace->insert(Replacement(
                    *Result.SourceManager,
                    CharSourceRange::getTokenRange(
                            SourceRange(member->getMemberLoc())), NewMethodName));
        }
    };
}

int main(int argc, char **argv) {
    cl::ParseCommandLineOptions(argc, argv);
    string ErrorMessage;
    std::unique_ptr<CompilationDatabase> Compilations (
            CompilationDatabase::loadFromDirectory(
                    BuildPath, ErrorMessage));
    if (!Compilations)
        report_fatal_error(ErrorMessage);

    RefactoringTool Tool(*Compilations, SourcePaths);
    ast_matchers::MatchFinder Finder;
    ChangeMemberDecl Callback1(&Tool.getReplacements());
    ChangeMemberCall Callback2(&Tool.getReplacements());

    Finder.addMatcher(
            recordDecl(
                    allOf(hasMethod(id("methodDecl",
                                       methodDecl(hasName(OriginalMethodName)))),
                          isSameOrDerivedFrom(hasName(ClassName)))),
            &Callback1);
    Finder.addMatcher(
            memberCallExpr(
                    callee(id("member",
                              memberExpr(member(hasName(OriginalMethodName))))),
                    thisPointerType(recordDecl(
                            isSameOrDerivedFrom(hasName(ClassName))))),
            &Callback2);
    return Tool.runAndSave(newFrontendActionFactory(&Finder).get());
}
*/

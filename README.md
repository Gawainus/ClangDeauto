This is a clang tool that reports auto typed declaration in C++ code.

Milestones:

1. Works fine for built-in types, int, double, etc.

2. User can use -report option to review outputs before writing to the existing files.

3. Whether auto str = "blah" should be std::string or char array is an issue.

4. std::function or function pointer is a choice

Current Issues:

1. Range-based loop iterator cannot be inferred.

2. virtual function is not inferred correctly.

Test Plan:

Clang has the unit test facilities, which I have not looked at. For next week, I would like to make it work first which means for trivial auto type specifier, my tool can deduce the underlying type.

It seems that clang has its own fashion of unittesting based on the in-package tests for tools like unittests/ASTMatchers.

Possible types:
1. built-in types: int, double, etc.

2. Templated types.

3. Pointers

4. std::function

References:

Tutorial for building tools using LibTooling and LibASTMatchers
http://clang.llvm.org/docs/LibASTMatchersTutorial.html

AST Matcher Reference
http://clang.llvm.org/docs/LibASTMatchersReference.html

Clang Tool Class Reference
The RefactoringTool class inherits from ClangTool class.
http://clang.llvm.org/doxygen/classclang_1_1tooling_1_1ClangTool.html


![alt tag](https://upload.wikimedia.org/wikipedia/en/4/4c/LLVM_Logo.svg)

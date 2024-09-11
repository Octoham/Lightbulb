#include "mincc.h"

using namespace std;
using namespace llvm;

int main(int argc, char* argv[]) {
    // Handle arguments
    cout << "Number of arguments: " << argc << endl;

    for (int i = 0; i < argc; i++) {
        cout << "Argument " << i << ": " << argv[i] << endl;
    }

    if (argc > 1)
    {
        if (argv[1] == "test")
        {
            LLVMCodeGenTest(argv[1]);
        }
        else
        {
            //Lexer lexer = Lexer(ReadFile("D:\\repos\\Lightbulb\\testfiles\\mincc\\test2.c"));
            Lexer lexer = Lexer(ReadFile(argv[1]), argv[1]);
            lexer.FixLineEnd();
            vector<Token> tokens = lexer.Lex();
            lexer.PrintTokens(tokens);
        }
    }
    // TODO Call lexer, then parser, then IRgen

    return 0;
}

string ReadFile(string path)
{
    // Simple function to easily open a file
    ifstream file(path);
    if (file.is_open()) {
        string contents((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
        //cout << contents << endl; // Print the contents for now
        file.close();
        return contents;
    }
    else {
        cerr << "Unable to open file";
        return 0;
    }
}

void LLVMCodeGenTest(string path)
{
    LLVMContext Context; // make context
    Module* M = new Module("test_module", Context); // make test module

    Function* F = Function::Create(
        FunctionType::get(Type::getInt32Ty(Context), {}, false),
        Function::ExternalLinkage,
        "test_function",
        M); // make test function

    BasicBlock* BB = BasicBlock::Create(Context, "test_entry", F); // make basic code block (function) to be used as entrypoint

    IRBuilder<> Builder(BB); // init ir builder
    auto namet = Builder.CreateAdd(ConstantInt::get(Type::getInt32Ty(Context), 9), ConstantInt::get(Type::getInt32Ty(Context), 10), "namet");
    Builder.CreateRet(namet); // make simple return statement

    verifyModule(*M, &errs()); // verify that the module is valid

    InitializeAllTargetInfos(); // init registries of targets and whatnot
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    string TT = sys::getDefaultTargetTriple(); // set target triple to our current machine
    M->setTargetTriple(TT); // use that target triple for the module

    string E;
    const Target* T = TargetRegistry::lookupTarget(TT, E); // lookup the target triple in the triple registry
    // Print an error and exit if we couldn't find the requested target.
    // This generally occurs if we've forgotten to initialise the
    // TargetRegistry or we have a bogus target triple.
    if (!T) {
        errs() << E;
        return;
    }

    StringRef CPU = sys::getHostCPUName();
    SubtargetFeatures Feat;
#if false // only set to true on latest llvm
    StringMap<bool> HF = sys::getHostCPUFeatures();
    if (HF.empty())
#else
    StringMap<bool> HF;
    if(sys::getHostCPUFeatures(HF))
#endif
        for (auto& F : HF)
            Feat.AddFeature(F.first(), F.second); // list every single feature
    string Features = Feat.getString(); // actually get features

    cout << "Current triple : " << T->getName() << endl;
    cout << "Current CPU: " << CPU.str() << endl;
    cout << "Currently enabled features: " << Features << endl;

    TargetOptions O;
    TargetMachine* TM = T->createTargetMachine(TT, CPU, Features, O, Reloc::PIC_); // actually create the target machine
    M->setDataLayout(TM->createDataLayout()); // set the data layout

    M->print(errs(), nullptr); // print the module

    error_code EC;
    raw_fd_ostream dest(path, EC, sys::fs::OF_None); // make an ostream to the path

    legacy::PassManager pass;
    auto FileType = CodeGenFileType::ObjectFile;

    if (TM->addPassesToEmitFile(pass, dest, nullptr, FileType)) // set output pass to the destination file
    {
        errs() << "TargetMachine can't emit a file of this type";
        return;
    }
    pass.run(*M); // acually run the output pass
    dest.flush();

    //std::unique_ptr<MemoryBuffer> Buffer = MemoryBuffer::getMemBuffer();

}

/*============================================================================*/
/* CompileLib.hpp - Dynamic code compiler and loader                          */
/*============================================================================*/

#include <dlfcn.h>
#include "BASE.h"
#include "CompileLib.hpp"
#include "LibInfo.hpp"
#include "eCB.h"
#include "Shell.hpp"
#include "SysCall.hpp"

using namespace STORE;

static Stz* C_QUERYLIBS_PATH = "../queryLibs"_Stz;
static Stz* C_QUERYLIBS_SRC  = "../queryLibs/src"_Stz;
static Stz* C_QUERYLIBS_BIN  = "../queryLibs/bin"_Stz;

static Stz* C_STARTUP_DIRS[] = {
    C_QUERYLIBS_PATH,
    C_QUERYLIBS_SRC,
    C_QUERYLIBS_BIN
};

static eCB C_DEFAULT_COMPILER = CB_NONE;
static Stz* C_DEFAULT_COMPILER_PATH = nullptr;

static Stz* C_GCC_COMPILER_FLAGS = "-std=c++11 -g -Wl,--unresolved-symbols=ignore-in-shared-libs -I../src -O3 -fpic -shared"_Stz;
static Stz* C_CLANG_COMPILER_FLAGS = "-std=c++11 -g -Wl,-undefined,dynamic_lookup -I../src -O3 -fpic -shared"_Stz;

static Stz* C_TCC_PATH = "../thirdparty/tcc/bin/tcc"_Stz;
static Stz* C_TCC_COMPILER_FLAGS = "-g -I../src -shared"_Stz;

static Stz* C_GPLUSPLUS = "g++"_Stz;
static Stz* C_CLANGPLUSPLUS = "clang++"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public Functions---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Initialise the CompileLib and look for all supported compilers.            */
/*----------------------------------------------------------------------------*/
bool CompileLib::bInit()
{
  // Create the queryLibs directory and subdirectories
  for (Stz* szDirPath : C_STARTUP_DIRS) {
    if (!Dir::bMake(szDirPath)) {
      vTrace("Unable to create directory %s\n"_Stz, szDirPath);
      return false;
    }
  }

  // TODO: Determine what compilers are available on the machine and set
  // TODO: C_DEFAULT_COMPILER and C_DEFAULT_COMPILER_VER appropriately.
  // TODO: Also log the known compilers to the log file for debugging purposes.

  Stz* szCompilerExistsCmd;

#ifdef WITH_APPLE
  C_DEFAULT_COMPILER = CB_CLANG;
  C_DEFAULT_COMPILER_PATH = C_CLANGPLUSPLUS;
  szCompilerExistsCmd = "which clang++ &>/dev/null"_Stz;
#endif

#ifdef WITH_LINUX
  C_DEFAULT_COMPILER = CB_GCC;
  C_DEFAULT_COMPILER_PATH = C_GPLUSPLUS;
  szCompilerExistsCmd = "which g++ &>/dev/null"_Stz;

  // TODO: Check if we are on CentOS 7, if SCL is available, and if gcc
  //       or clang are installed with it. We would probably prefer this
  //       over the built-in gcc version.
#endif

  // Check that the default compiler exists. SE will fail early if it cannot be found.
  SysCall oSysCall;
  if (Shell::iExecute(szCompilerExistsCmd, oSysCall) != 0) {
    vTrace("Default compiler not found. RapidsSE requires a compiler for executing queries. "
           "The compiler %s could not be found with the command \"%s\".\n"_Stz,
           C_DEFAULT_COMPILER_PATH, szCompilerExistsCmd);
    return false;
  }

  return true;
}

// TODO: Provide a function to list the known compilers


/*----------------------------------------------------------------------------*/
/* Compiler source code to a dynamic library. Returns true on success.        */
/*----------------------------------------------------------------------------*/
bool CompileLib::bCompile(Stz* szSource, Stz* szDynLibNm, LibInfo* pLibInfo, Stz* szCompiler)
{
  ASSERT(szSource);
  ASSERT(szDynLibNm);
  ASSERT(pLibInfo);
  ASSERT(!pLibInfo->m_pvDynLib);
  ASSERT(!szCompiler || !szCompiler->bIsEmpty());
  pLibInfo->vClearErrors();

  u8 uStartTime;
  if (pLibInfo->m_bCollectStats) {
    uStartTime = Hardware::uReadClock();
  }

  // Choose or validate a compiler, if necessary
  if (!bValidateCompiler(szCompiler, pLibInfo)) {
    return false;
  }

  // Save the base name
  pLibInfo->m_sBaseName.vCopy(szDynLibNm);

  // Come up with file path for the source file and output file
  pLibInfo->m_sSrcFilePath.vSprint("%s/%s.c"_Stz, C_QUERYLIBS_SRC, pLibInfo->m_sBaseName.szStz());
  pLibInfo->m_sDynLibPath.vSprint("%s/lib%s.so"_Stz, C_QUERYLIBS_BIN, pLibInfo->m_sBaseName.szStz());
  pLibInfo->m_sLogFilePath.vSprint("%s/%s.log"_Stz, C_QUERYLIBS_SRC, pLibInfo->m_sBaseName.szStz());

  // Write the source file to disk
  if (!bWriteSrcToFile(szSource, pLibInfo)) {
    return false;
  }

  // Build the compile command
  switch (pLibInfo->m_eCompilerType) {
    case CB_GCC:        vBuildGccCompileCommand(pLibInfo);    break;
    case CB_CLANG:      vBuildClangCompileCommand(pLibInfo);  break;
    case CB_TCC:        vBuildTccCompileCommand(pLibInfo);    break;
    case CB_SCL_GCC:    vBuildSclGccCompileCommand(pLibInfo); break;
    case CB_SCL_CLANG:  vBuildSclClangCompileCommand(pLibInfo); break;

    default:
      NEVER_REACH
  }

  // Execute the compile command
  if (!bCompileSrc(pLibInfo)) {
    return false;
  }

  // Check that the library exists
  ASSERT(File::bExists(pLibInfo->m_sDynLibPath.szStz()));

  if (pLibInfo->m_bCollectStats) {
    pLibInfo->m_uCompileNanos = Hardware::uReadClock() - uStartTime;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* Dynamically load a shared object library. Returns true on success.         */
/*----------------------------------------------------------------------------*/
bool CompileLib::bDynamicLink(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  ASSERT(!pLibInfo->m_pvDynLib);
  ASSERT(!pLibInfo->m_sDynLibPath.bIsEmpty());
  pLibInfo->vClearErrors();

  u8 uStartTime = 0;
  if (pLibInfo->m_bCollectStats) {
    uStartTime = Hardware::uReadClock();
  }

  pLibInfo->m_pvDynLib = dlopen(pLibInfo->m_sDynLibPath.azChar(), RTLD_NOW);

  if (!pLibInfo->m_pvDynLib) {
    pLibInfo->m_eErrCode = EC_DYN_LINK_FAILED;

    pLibInfo->m_sErrDetails.vCopy((Stz*) dlerror());
    pLibInfo->m_sErrDetails.vCatSprint(" : %s"_Stz, pLibInfo->m_sDynLibPath.szStz());

    return false;
  }

  if (pLibInfo->m_bCollectStats) {
    pLibInfo->m_uDynLinkNanos = Hardware::uReadClock() - uStartTime;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* Get the address of a symbol in the loaded library, or nullptr if error     */
/*----------------------------------------------------------------------------*/
void* CompileLib::pvSymbolAddr(Stz* szSymbolNm, LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  ASSERT(pLibInfo->m_pvDynLib);
  ASSERT(szSymbolNm);
  pLibInfo->vClearErrors();

  u8 uStartTime;
  if (pLibInfo->m_bCollectStats) {
    uStartTime = Hardware::uReadClock();
  }

  dlerror();  // resets any previous error
  void* pvPtr = dlsym(pLibInfo->m_pvDynLib, szSymbolNm->azChar());

  if (!pvPtr) {
    pLibInfo->m_eErrCode = EC_DYN_SYM_RESOLVE_FAILED;

    pLibInfo->m_sErrDetails.vCopy((Stz*) dlerror());
    pLibInfo->m_sErrDetails.vCatSprint(" : %s : %s"_Stz,
                                       pLibInfo->m_sDynLibPath.szStz(),
                                       szSymbolNm);
  }

  if (pLibInfo->m_bCollectStats) {
    pLibInfo->m_uSymAddrNanos = Hardware::uReadClock() - uStartTime;
  }

  return pvPtr;
}

/*----------------------------------------------------------------------------*/
/* Dynamically unload a shared object library. Returns true on success.       */
/*----------------------------------------------------------------------------*/
bool CompileLib::bDynamicUnlink(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  ASSERT(pLibInfo->m_pvDynLib);
  pLibInfo->vClearErrors();

  u8 uStartTime;
  if (pLibInfo->m_bCollectStats) {
    uStartTime = Hardware::uReadClock();
  }

  i8 iVal = dlclose(pLibInfo->m_pvDynLib);
  if (iVal != 0) {
    pLibInfo->m_eErrCode = EC_DYN_UNLINK_FAILED;

    pLibInfo->m_sErrDetails.vCopy((Stz*) dlerror());
    pLibInfo->m_sErrDetails.vCatSprint(" : %s"_Stz, pLibInfo->m_sDynLibPath.szStz());
  }

  pLibInfo->m_pvDynLib = nullptr;

  if (pLibInfo->m_bCollectStats) {
    pLibInfo->m_uDynUnlinkNanos = Hardware::uReadClock() - uStartTime;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* Delete a shared object library & associated files. Returns true on success.*/
/*----------------------------------------------------------------------------*/
void CompileLib::vDelete(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  ASSERT(!pLibInfo->m_pvDynLib);
  pLibInfo->vClearErrors();

  u8 uStartTime;
  if (pLibInfo->m_bCollectStats) {
    uStartTime = Hardware::uReadClock();
  }

  ASSERT(pLibInfo->m_pvDynLib == nullptr);

  // Remove shared object, if exists
  if (!pLibInfo->m_sDynLibPath.bIsEmpty()) {
    File::bRemove(pLibInfo->m_sDynLibPath.szStz(), true);
  }

#ifdef WITH_APPLE
  // MacOS seems to drop a directory with symbols for the compiled shared library.
  // This is the same name/path as the .so but with an additional extension .dSYM.
  s256 sCmd;
  sCmd.vSprint("rm -rf %s.dSYM"_Stz, pLibInfo->m_sDynLibPath.szStz());
  SysCall oSysCall;
  Shell::iExecute(sCmd.szStz(), oSysCall);
#endif

  // Remove log file, if exists
  if (!pLibInfo->m_sLogFilePath.bIsEmpty()) {
    File::bRemove(pLibInfo->m_sLogFilePath.szStz(), true);
  }

  // Remove source file, if exists
  if (!pLibInfo->m_sSrcFilePath.bIsEmpty()) {
    File::bRemove(pLibInfo->m_sSrcFilePath.szStz(), true);
  }

  if (pLibInfo->m_bCollectStats) {
    pLibInfo->m_uDeleteNanos = Hardware::uReadClock() - uStartTime;
  }
}

/*----------------------------------------------------------------------------*/
/* Temporary stub to test this stuff out.                                     */
/*----------------------------------------------------------------------------*/
void CompileLib::vTest()
{
  static Stz* szSrc =
"#include <stdio.h>\n"
"#include \"primitives/BaseTypes_tcc.h\"\n"
"\n"
"// Declaration of a function inside SE\n"
"// This only compiles because we are supplying a linker flag\n"
"// to ignore unresolved symbols. So if SE does not provide this\n"
"// symbol then this call will crash.\n"
"extern void vTraceExternal(char* psFormat, ...);\n"
"\n"
"extern \"C\" void vTestEntry(void)\n"
"{\n"
"  printf(\"Inside vTestEntry!\\n\");\n"
"\n"
"  char* psMsg = \"Calling into SE from a shared object!\\n\";\n"
"  vTraceExternal(psMsg);\n"
"\n"
"  printf(\"Leaving vTestEntry!\\n\");\n"
"}"_Stz;


  static Stz* szDynLibNm = "test"_Stz;
  static Stz* szFuncNm = "vTestEntry"_Stz;
  LibInfo* pLibInfo = new LibInfo(true);

  // The function pointer we want to call in the shared library
  void (*pfTestFn)(void);

  if (!bCompile(szSrc, szDynLibNm, pLibInfo)) {
    vTrace("Compilation failed\n"_Stz);

    vDelete(pLibInfo);
    delete pLibInfo;
    return;
  }

  if (!bDynamicLink(pLibInfo)) {
    vTrace("Dynamic linking & loading failed\n"_Stz);

    vDelete(pLibInfo);
    delete pLibInfo;
    return;
  }

  // We use this rather clumsy way to cast the void* returned by pvSymbolAddr
  // to a function pointer defined above, because the ISO C standard leaves the
  // result of casting between function pointers and void* undefined. This approach
  // also avoids compiler warnings if we simply try to cast the function ptr
  // to a void* for the assignment.
  // Reference: The Linux Programming Interface, Chapter 42.1.3

  *(void **)(&pfTestFn) = pvSymbolAddr(szFuncNm, pLibInfo);

  if (!pfTestFn) {
    vTrace("Symbol resolution failed of symbol %s\n"_Stz, szFuncNm);

    if (!CompileLib::bDynamicUnlink(pLibInfo)) {
      vTrace("Dynamic unlink failed\n"_Stz);
    }

    vDelete(pLibInfo);
    delete pLibInfo;
    return;
  }

  // Call the function
  (*pfTestFn)();

  // Unlink
  if (!bDynamicUnlink(pLibInfo)) {
    vTrace("Dynamic unlinking failed\n"_Stz);

    vDelete(pLibInfo);
    delete pLibInfo;
    return;
  }

  // Cleanup temporary files
  vDelete(pLibInfo);

  vTrace("\n  Compile time:  %lluns"
         "\n  Dyn Link time: %lluns"
         "\n  Sym Addr time: %lluns"
         "\n  Dyn Unload   : %lluns"
         "\n  Delete time  : %lluns"
         "\n\n"_Stz,
         pLibInfo->uCompileNanos(),
         pLibInfo->uDynLinkNanos(),
         pLibInfo->uSymAddrNanos(),
         pLibInfo->uDynUnlinkNanos(),
         pLibInfo->uDeleteNanos()
  );

  delete pLibInfo;
}


/*----------------------------------------------------------------------------*/
/*-- Private Functions--------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Validate the compiler requested                                            */
/*----------------------------------------------------------------------------*/
bool CompileLib::bValidateCompiler(Stz* szCompiler, LibInfo* pLibInfo)
{
  ASSERT(!szCompiler || !szCompiler->bIsEmpty());
  ASSERT(pLibInfo);
  ASSERT(pLibInfo->m_eCompilerType == CB_NONE);

  // Default compiler?
  if (!szCompiler) {
    // No compiler specified, use the default one for the current platform
    pLibInfo->m_eCompilerType  = C_DEFAULT_COMPILER;
    pLibInfo->m_sCompilerPath = C_DEFAULT_COMPILER_PATH;
    return true;
  }

  // Do we have an absolute pathname?
  if (szCompiler->bBeginsWith("/"_Stz)) {
    // We still need to know what type of compiler it is (gcc, clang, tcc, other?)
    if      (szCompiler->bContains("g++"_Stz))      { pLibInfo->m_eCompilerType = CB_GCC;   }
    else if (szCompiler->bContains("gcc"_Stz))      { pLibInfo->m_eCompilerType = CB_GCC;   }
    else if (szCompiler->bContains("clang++"_Stz))  { pLibInfo->m_eCompilerType = CB_CLANG; }
    else if (szCompiler->bContains("clang"_Stz))    { pLibInfo->m_eCompilerType = CB_CLANG; }
    else if (szCompiler->bContains("tcc"_Stz))      { pLibInfo->m_eCompilerType = CB_TCC;   }
    else {
      // Unknown/unsupported compiler. We don't know how to call it, what flags or
      // arguments it accepts, etc.
      pLibInfo->m_eErrCode = EC_COMPILATION_FAILED;
      pLibInfo->m_sErrDetails.vSprint("Unrecognised or unsupported compiler '%s'"_Stz, szCompiler);
      return false;
    }

    // Save the path of this compiler
    pLibInfo->m_sCompilerPath = szCompiler;

    // Check that the compiler exists
    return bCheckCompilerExists(szCompiler, pLibInfo);
  }

  // Do we have a compiler mnemonic?
  // If so, translate this into a compiler type and path.

  if (szCompiler->bEqual("gcc"_Stz) || szCompiler->bEqual("g++"_Stz)) {
    pLibInfo->m_eCompilerType = CB_GCC;
    pLibInfo->m_sCompilerPath = C_GPLUSPLUS;
    return bCheckCompilerExists(C_GPLUSPLUS, pLibInfo);

  } else if (szCompiler->bBeginsWith("clang"_Stz) || szCompiler->bBeginsWith("clang++"_Stz)) {
    pLibInfo->m_eCompilerType = CB_CLANG;
    pLibInfo->m_sCompilerPath = C_CLANGPLUSPLUS;
    return bCheckCompilerExists(C_CLANGPLUSPLUS, pLibInfo);

  } else if (szCompiler->bEqual("tcc"_Stz)) {
#ifdef WITH_APPLE
    // tcc is not supported when it is being invoked as an external tool
    // on MacOS. Can't get it to easily include header files and link against
    // system libraries.
    pLibInfo->m_eErrCode = EC_COMPILATION_FAILED;
    pLibInfo->m_sErrDetails.vCopy("tcc compiler is not available on MacOS"_Stz);
    return false;
#endif

    pLibInfo->m_eCompilerType = CB_TCC;
    pLibInfo->m_sCompilerPath = C_TCC_PATH;
    return bCheckCompilerExists(C_TCC_PATH, pLibInfo);

  } else if (szCompiler->bBeginsWith("devtoolset-"_Stz)) {
    pLibInfo->m_eCompilerType = CB_SCL_GCC;
    pLibInfo->m_sCompilerPath = C_GPLUSPLUS;
    return bCheckCompilerExists(szCompiler, C_GPLUSPLUS, pLibInfo);

  } else if (szCompiler->bBeginsWith("llvm-toolset-"_Stz)) {
    pLibInfo->m_eCompilerType = CB_SCL_CLANG;
    pLibInfo->m_sCompilerPath = C_CLANGPLUSPLUS;
    return bCheckCompilerExists(szCompiler, C_CLANGPLUSPLUS, pLibInfo);
  }

  // Unknown compiler
  pLibInfo->m_eErrCode = EC_COMPILATION_FAILED;
  pLibInfo->m_sErrDetails.vSprint("Unrecognised or unsupported compiler '%s'"_Stz, szCompiler);
  return false;
}

/*----------------------------------------------------------------------------*/
/* Check whether the compiler binary can be found. Sets an error msg if not   */
/*----------------------------------------------------------------------------*/
bool CompileLib::bCheckCompilerExists(Stz* szCompiler, LibInfo* pLibInfo)
{
  s256 sCmd;
  SysCall oSysCall;

  // Is it on the $PATH?
  sCmd.vSprint("which %s &>/dev/null"_Stz, szCompiler);

  if (Shell::iExecute(sCmd.szStz(), oSysCall) == 0) {
    return true;
  }

  // Can it be resolved as a relative or absolute path?
  sCmd.vSprint("ls %s &>/dev/null"_Stz, szCompiler);
  if (Shell::iExecute(sCmd.szStz(), oSysCall) == 0) {
    return true;
  }

  pLibInfo->m_eErrCode = EC_COMPILATION_FAILED;
  pLibInfo->m_sErrDetails.vSprint("Compiler not found at path '%s'"_Stz, szCompiler);
  return false;
}

/*----------------------------------------------------------------------------*/
/* Check whether the compiler binary can be found. Sets an error msg if not   */
/*----------------------------------------------------------------------------*/
bool CompileLib::bCheckCompilerExists(Stz* szSclEnv, Stz* szCompiler, LibInfo* pLibInfo)
{
  s256 sCmd;
  SysCall oSysCall;

  // Is it on the $PATH?
  sCmd.vSprint("scl enable %s 'which %s' &>/dev/null"_Stz, szSclEnv, szCompiler);

  if (Shell::iExecute(sCmd.szStz(), oSysCall) == 0) {
    return true;
  }

  pLibInfo->m_eErrCode = EC_COMPILATION_FAILED;
  pLibInfo->m_sErrDetails.vSprint("Compiler %s not found in SCL environment %s"_Stz, szCompiler, szSclEnv);
  return false;
}

/*----------------------------------------------------------------------------*/
/* Write the source string to a file as designated in pLibInfo                */
/*----------------------------------------------------------------------------*/
bool CompileLib::bWriteSrcToFile(Stz* szSource, LibInfo* pLibInfo)
{
  ASSERT(szSource);
  ASSERT(pLibInfo);
  ASSERT(!pLibInfo->m_sSrcFilePath.bIsEmpty());

  File* pSrcFile = new File(pLibInfo);
  if (!pSrcFile->bOpenWrite(pLibInfo->m_sSrcFilePath.szStz())) {
    pLibInfo->m_eErrCode = EC_COMPILATION_FAILED;

    strerror_r(pSrcFile->oSysCall().iError(), pLibInfo->m_sErrDetails.azChar(), pLibInfo->m_sErrDetails.nSpace());
    pLibInfo->m_sErrDetails.vCatSprint(" : %s"_Stz, pLibInfo->m_sSrcFilePath.szStz());

    delete pSrcFile;
    return false;
  }

  if (!pSrcFile->bWriteBytes(szSource, szSource->nLength())) {
    pLibInfo->m_eErrCode = EC_COMPILATION_FAILED;

    strerror_r(pSrcFile->oSysCall().iError(), pLibInfo->m_sErrDetails.azChar(), pLibInfo->m_sErrDetails.nSpace());
    pLibInfo->m_sErrDetails.vCatSprint(" : %s"_Stz, pLibInfo->m_sSrcFilePath.szStz());

    pSrcFile->vClose();
    delete pSrcFile;
    return false;
  }

  pSrcFile->vClose();
  delete pSrcFile;
  return true;
}

/*----------------------------------------------------------------------------*/
/* Generate the compilation command for gcc                                   */
/*----------------------------------------------------------------------------*/
void CompileLib::vBuildGccCompileCommand(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  ASSERT(!pLibInfo->m_sSrcFilePath.bIsEmpty());
  ASSERT(!pLibInfo->m_sDynLibPath.bIsEmpty());
  ASSERT(!pLibInfo->m_sLogFilePath.bIsEmpty());
  ASSERT(!pLibInfo->m_pvDynLib);

  pLibInfo->m_sCompileCmd.vReset();

  pLibInfo->m_sCompileCmd.vSprint("%s %s -o %s %s > %s 2>&1"_Stz,
                                  pLibInfo->m_sCompilerPath.szStz(),
                                  C_GCC_COMPILER_FLAGS,
                                  pLibInfo->m_sDynLibPath.szStz(),
                                  pLibInfo->m_sSrcFilePath.szStz(),
                                  pLibInfo->m_sLogFilePath.szStz());
}

/*----------------------------------------------------------------------------*/
/* Generate the compilation command for clang                                 */
/*----------------------------------------------------------------------------*/
void CompileLib::vBuildClangCompileCommand(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  ASSERT(!pLibInfo->m_sSrcFilePath.bIsEmpty());
  ASSERT(!pLibInfo->m_sDynLibPath.bIsEmpty());
  ASSERT(!pLibInfo->m_sLogFilePath.bIsEmpty());
  ASSERT(!pLibInfo->m_pvDynLib);

  // Clang command line arguments reference:
  // https://clang.llvm.org/docs/ClangCommandLineReference.html

  pLibInfo->m_sCompileCmd.vReset();

  pLibInfo->m_sCompileCmd.vSprint("%s %s -o %s %s > %s 2>&1"_Stz,
                                  pLibInfo->m_sCompilerPath.szStz(),
                                  C_CLANG_COMPILER_FLAGS,
                                  pLibInfo->m_sDynLibPath.szStz(),
                                  pLibInfo->m_sSrcFilePath.szStz(),
                                  pLibInfo->m_sLogFilePath.szStz());
}

/*----------------------------------------------------------------------------*/
/* Generate the compilation command for the built-in tcc                      */
/*----------------------------------------------------------------------------*/
void CompileLib::vBuildTccCompileCommand(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  ASSERT(!pLibInfo->m_sSrcFilePath.bIsEmpty());
  ASSERT(!pLibInfo->m_sDynLibPath.bIsEmpty());
  ASSERT(!pLibInfo->m_sLogFilePath.bIsEmpty());
  ASSERT(!pLibInfo->m_pvDynLib);

  pLibInfo->m_sCompileCmd.vReset();

  pLibInfo->m_sCompileCmd.vSprint("%s %s -o %s %s > %s 2>&1"_Stz,
                                  pLibInfo->m_sCompilerPath.szStz(),
                                  C_TCC_COMPILER_FLAGS,
                                  pLibInfo->m_sDynLibPath.szStz(),
                                  pLibInfo->m_sSrcFilePath.szStz(),
                                  pLibInfo->m_sLogFilePath.szStz());
}

/*----------------------------------------------------------------------------*/
/* Generate the compilation command for gcc operating under SCL on CentOS     */
/*----------------------------------------------------------------------------*/
void CompileLib::vBuildSclGccCompileCommand(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  ASSERT(!pLibInfo->m_sSrcFilePath.bIsEmpty());
  ASSERT(!pLibInfo->m_sDynLibPath.bIsEmpty());
  ASSERT(!pLibInfo->m_sLogFilePath.bIsEmpty());
  ASSERT(!pLibInfo->m_pvDynLib);

  // szCompiler is typically "devtoolset-10", "devtoolset-9", ...

  pLibInfo->m_sCompileCmd.vReset();

  pLibInfo->m_sCompileCmd.vSprint("scl enable %s '%s %s -o %s %s' > %s 2>&1"_Stz,
                                  pLibInfo->m_sCompilerPath.szStz(),
                                  C_GPLUSPLUS,
                                  C_GCC_COMPILER_FLAGS,
                                  pLibInfo->m_sDynLibPath.szStz(),
                                  pLibInfo->m_sSrcFilePath.szStz(),
                                  pLibInfo->m_sLogFilePath.szStz());
}

/*----------------------------------------------------------------------------*/
/* Generate the compilation command for clang operating under SCL on CentOS   */
/*----------------------------------------------------------------------------*/
void CompileLib::vBuildSclClangCompileCommand(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  ASSERT(!pLibInfo->m_sSrcFilePath.bIsEmpty());
  ASSERT(!pLibInfo->m_sDynLibPath.bIsEmpty());
  ASSERT(!pLibInfo->m_sLogFilePath.bIsEmpty());
  ASSERT(!pLibInfo->m_pvDynLib);

  // szCompiler is typically llvm-toolset-7.0-clang.

  pLibInfo->m_sCompileCmd.vReset();

  pLibInfo->m_sCompileCmd.vSprint("scl enable %s '%s %s -o %s %s' > %s 2>&1"_Stz,
                                  pLibInfo->m_sCompilerPath.szStz(),
                                  C_CLANGPLUSPLUS,
                                  C_CLANG_COMPILER_FLAGS,
                                  pLibInfo->m_sDynLibPath.szStz(),
                                  pLibInfo->m_sSrcFilePath.szStz(),
                                  pLibInfo->m_sLogFilePath.szStz());
}

/*----------------------------------------------------------------------------*/
/* Compile the source to a shared object. This blocks until it is complete.   */
/*----------------------------------------------------------------------------*/
bool CompileLib::bCompileSrc(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  ASSERT(!pLibInfo->m_sCompileCmd.bIsEmpty());
  ASSERT(!pLibInfo->m_pvDynLib);

  SysCall oSysCall;
  i8 iExitCode = Shell::iExecute(pLibInfo->m_sCompileCmd.szStz(), oSysCall);

  if (iExitCode != 0) {
    pLibInfo->m_eErrCode = EC_COMPILATION_FAILED;
    pLibInfo->m_sErrDetails.vSprint("Compilation failed with exit code %lld"_Stz, iExitCode);
    return false;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
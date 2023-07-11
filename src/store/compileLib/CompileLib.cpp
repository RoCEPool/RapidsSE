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

#include "ExtendedText.hpp"

using namespace STORE;

static Stz* C_QUERYLIBS_PATH = "../queryLibs"_Stz;
static Stz* C_QUERYLIBS_SRC  = "../queryLibs/src"_Stz;
static Stz* C_QUERYLIBS_BIN  = "../queryLibs/bin"_Stz;

static Stz* C_STARTUP_DIRS[] = {
    C_QUERYLIBS_PATH,
    C_QUERYLIBS_SRC,
    C_QUERYLIBS_BIN
};

#ifdef WITH_LINUX
static Stz* C_LIB_EXT = "so"_Stz;
#endif
#ifdef WITH_APPLE
static Stz* C_LIB_EXT = "dylib"_Stz;
#endif

static eCB C_DEFAULT_COMPILER = CB_NONE;
static Stz* C_DEFAULT_COMPILER_PATH = nullptr;

static Stz* C_GCC_COMPILER_FLAGS = "-g -Wall -Wextra -Wno-unused-function -Wl,--unresolved-symbols=ignore-all  -Werror=return-type -I../src -O3 -fpic -shared"_Stz;

#ifdef WITH_DEBUG_VERSION
  static Stz* C_CLANG_COMPILER_FLAGS = "-DWITH_DEBUG_VERSION -g -Wall -Wextra -Wno-unused-function -Wl,-undefined,dynamic_lookup -Werror=return-type -I../src -O0 -fpic -shared"_Stz;
#else
  static Stz* C_CLANG_COMPILER_FLAGS = "-g -Wall -Wextra -Wno-unused-function -Wl,-undefined,dynamic_lookup -Werror=return-type -I../src -O3 -fpic -shared"_Stz;
#endif

static Stz* C_TCC_PATH = "../thirdparty/tcc/bin/tcc"_Stz;

#ifdef WITH_LINUX
static Stz* C_TCC_COMPILER_FLAGS = "-B../thirdparty/tcc/lib/tcc -g -I../src -rdynamic -shared"_Stz;
#endif
#ifdef WITH_APPLE
// tcc cannot find the header files and library files because they are not in a standard place.
// We can try to help it, but specifying the options below (which is likely not portable),
// but we still get issues with:
//  * An unsupported compiler warning on system header cdefs.h
//  * An error that library 'c' and file 'crtn.o' not found.
// It also makes the compile command line much bigger, so we need to increase the size of
// LibInfo::m_sCompileCmd from s256 to s2K (and therefore LibInfo from the small to the medium heap).
// Leaving this in here for posterity.
static Stz* C_TCC_COMPILER_FLAGS =
    "-B../thirdparty/tcc/lib/tcc -g -I../src "
    "-isystem /usr/local/include"
    "-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1 "
    "-isystem /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/13.0.0/include "
    "-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include "
    "-isystem /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include "
    "-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks "
    "-L/usr/local/lib "
    "-L/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/lib "
    "-nostdlib "
    "-rdynamic -shared"_Stz;
#endif

static Stz* C_GCC = "gcc"_Stz;
//static Stz* C_GPLUSPLUS = "g++"_Stz;
static Stz* C_CLANG = "clang"_Stz;
//static Stz* C_CLANGPLUSPLUS = "clang++"_Stz;

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
  C_DEFAULT_COMPILER_PATH = C_CLANG;
  szCompilerExistsCmd = "which clang &>/dev/null"_Stz;
#endif

#ifdef WITH_LINUX
  C_DEFAULT_COMPILER = CB_GCC;
  C_DEFAULT_COMPILER_PATH = C_GCC;
  szCompilerExistsCmd = "which gcc &>/dev/null"_Stz;

  // TODO: Check if we are on CentOS 7, if SCL is available, and if gcc
  //       or clang are installed with it. We would probably prefer this
  //       over the built-in gcc version.
#endif

  // Check that the default compiler exists. SE will fail early if it cannot be found.
  s48K sStdOut;
  if (Shell::iExecute(szCompilerExistsCmd, sStdOut) != 0) {
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
  pLibInfo->m_sDynLibPath.vSprint("%s/lib%s.%s"_Stz, C_QUERYLIBS_BIN, pLibInfo->m_sBaseName.szStz(), C_LIB_EXT);
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
/* Compiler source code to a dynamic library. Returns true on success.        */
/*----------------------------------------------------------------------------*/
bool CompileLib::bCompile(ExtendedText* pSource, Stz* szDynLibNm, LibInfo* pLibInfo, Stz* szCompiler)
{
  ASSERT(pSource);
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
  pLibInfo->m_sDynLibPath.vSprint("%s/lib%s.%s"_Stz, C_QUERYLIBS_BIN, pLibInfo->m_sBaseName.szStz(), C_LIB_EXT);
  pLibInfo->m_sLogFilePath.vSprint("%s/%s.log"_Stz, C_QUERYLIBS_SRC, pLibInfo->m_sBaseName.szStz());

  // Write the source file to disk
  if (!bWriteSrcToFile(pSource, pLibInfo)) {
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

    // On failure save the src/log file in a different name
    // so that they don't get deleted and are copied across

    s256 sNewSrcFilePath;
    s256 sNewLogFilePath;

    sNewSrcFilePath.vSprint("%s/%s.c"_Stz, C_QUERYLIBS_SRC, "CompilationFailed");
    sNewLogFilePath.vSprint("%s/%s.log"_Stz, C_QUERYLIBS_SRC, "CompilationFailed");

    bool bResult = File::bRename(pLibInfo->m_sSrcFilePath, sNewSrcFilePath);
    if ( ! bResult )
      vTrace("Unable to rename file: %s"_Stz, pLibInfo->m_sSrcFilePath.azChar());

    bResult = File::bRename(pLibInfo->m_sLogFilePath, sNewLogFilePath);
    if ( ! bResult )
      vTrace("Unable to rename file: %s"_Stz, pLibInfo->m_sLogFilePath.azChar());

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

#ifdef WITH_RELEASE_VERSION
  // Remove log file, if exists
  if (!pLibInfo->m_sLogFilePath.bIsEmpty()) {
    File::bRemove(pLibInfo->m_sLogFilePath.szStz(), true);
  }

  // Remove source file, if exists
  if (!pLibInfo->m_sSrcFilePath.bIsEmpty()) {
    File::bRemove(pLibInfo->m_sSrcFilePath.szStz(), true);
  }
#endif

  if (pLibInfo->m_bCollectStats) {
    pLibInfo->m_uDeleteNanos = Hardware::uReadClock() - uStartTime;
  }
}

/*----------------------------------------------------------------------------*/
/* Temporary stub to test this stuff out.                                     */
/*----------------------------------------------------------------------------*/
void CompileLib::vTest(Stz* szCompiler)
{
  static Stz* szSrc =
"#include <stdio.h>\n"
"#include \"primitives/BaseTypes_tcc.h\"\n"
"\n"
"// Declaration of a function inside SE\n"
"// We either need to supply a basic implementation, or we need\n"
"// to give no implementation but supply a linker flag to ignore\n"
"// the unresolved linkage. If we give an implementation then we\n"
"// need to ensure that the entire function call is not optimised\n"
"// out, or we supply a compiler attribute to not optimise this function.\n"
"extern void vTraceExternal(char* psFormat, ...) __attribute__((weak_import));\n"
"\n"
"void vTestEntry(void)\n"
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

  // szCompiler can be set to:
  //   nullptr, "gcc", "clang", "devtoolset-10", "llvm-toolset-7.0", "/path/to/gcc", ...

  if (!CompileLib::bCompile(szSrc, szDynLibNm, pLibInfo, szCompiler)) {
    vTrace("Compilation failed: %s\n"_Stz, pLibInfo->szErrDetails());

    CompileLib::vDelete(pLibInfo);
    delete pLibInfo;
    return;
  }

  if (!CompileLib::bDynamicLink(pLibInfo)) {
    vTrace("Dynamic linking & loading failed: %s\n"_Stz, pLibInfo->szErrDetails());

    CompileLib::vDelete(pLibInfo);
    delete pLibInfo;
    return;
  }

  // We use this rather clumsy way to cast the void* returned by pvSymbolAddr
  // to a function pointer defined above, because the ISO C standard leaves the
  // result of casting between function pointers and void* undefined. This approach
  // also avoids compiler warnings if we simply try to cast the function ptr
  // to a void* for the assignment.
  // Reference: The Linux Programming Interface, Chapter 42.1.3

  *(void **)(&pfTestFn) = CompileLib::pvSymbolAddr(szFuncNm, pLibInfo);

  if (!pfTestFn) {
    vTrace("Symbol resolution failed of symbol %s: %s\n"_Stz, szFuncNm, pLibInfo->szErrDetails());

    if (!CompileLib::bDynamicUnlink(pLibInfo)) {
      vTrace("Dynamic unlink failed: %s\n"_Stz, pLibInfo->szErrDetails());
    }

    CompileLib::vDelete(pLibInfo);
    delete pLibInfo;
    return;
  }

  // Call the function
  (*pfTestFn)();

  // Unlink
  if (!CompileLib::bDynamicUnlink(pLibInfo)) {
    vTrace("Dynamic unlinking failed: %s\n"_Stz, pLibInfo->szErrDetails());

    vDelete(pLibInfo);
    delete pLibInfo;
    return;
  }

  // Cleanup temporary files
  CompileLib::vDelete(pLibInfo);

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

  // Do we have a compiler mnemonic?
  // If so, translate this into a compiler type and path.

  if (szCompiler->bEqual("gcc"_Stz) || szCompiler->bEqual("g++"_Stz)) {
    pLibInfo->m_eCompilerType = CB_GCC;
    pLibInfo->m_sCompilerPath = C_GCC;
    return bCheckCompilerExists(C_GCC, pLibInfo);

  } else if (szCompiler->bBeginsWith("clang"_Stz)) {
    pLibInfo->m_eCompilerType = CB_CLANG;
    pLibInfo->m_sCompilerPath = C_CLANG;
    return bCheckCompilerExists(C_CLANG, pLibInfo);

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
    pLibInfo->m_sCompilerPath = szCompiler;
    return bCheckCompilerExists(szCompiler, C_GCC, pLibInfo);

  } else if (szCompiler->bBeginsWith("llvm-toolset-"_Stz)) {
    pLibInfo->m_eCompilerType = CB_SCL_CLANG;
    pLibInfo->m_sCompilerPath = szCompiler;
    return bCheckCompilerExists(szCompiler, C_CLANG, pLibInfo);
  }

  // Do we have an absolute pathname?
  if (szCompiler->bBeginsWith("/"_Stz)) {
    // We still need to know what type of compiler it is (gcc, clang, tcc, other?)
    if      (szCompiler->bContains("g++"_Stz)) {
      pLibInfo->m_eCompilerType = CB_GCC;
      // We actually need to compile with gcc, so fix this in the compiler path
      StrLib::bReplace(pLibInfo->m_sCompilerPath.azChar(), "g++", "gcc");

    } else if (szCompiler->bContains("gcc"_Stz)) {
      pLibInfo->m_eCompilerType = CB_GCC;
      pLibInfo->m_sCompilerPath = szCompiler;

    } else if (szCompiler->bContains("clang++"_Stz)) {
      pLibInfo->m_eCompilerType = CB_CLANG;
      // We actually need to compile with clang, so fix this in the compiler path
      StrLib::bReplace(pLibInfo->m_sCompilerPath.azChar(), "clang++", "clang");

    } else if (szCompiler->bContains("clang"_Stz)) {
      pLibInfo->m_eCompilerType = CB_CLANG;
      pLibInfo->m_sCompilerPath = szCompiler;

    } else if (szCompiler->bContains("tcc"_Stz)) {
      pLibInfo->m_eCompilerType = CB_TCC;

    } else {
      // Unknown/unsupported compiler. We don't know how to call it, what flags or
      // arguments it accepts, etc.
      pLibInfo->m_eErrCode = EC_COMPILATION_FAILED;
      pLibInfo->m_sErrDetails.vSprint("Unrecognised or unsupported compiler '%s'"_Stz, szCompiler);
      return false;
    }

    // Save the path of this compiler if it hasn't already been set
    pLibInfo->m_sCompilerPath = szCompiler;

    // Check that the compiler exists
    return bCheckCompilerExists(szCompiler, pLibInfo);
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
  s48K sStdOut;

  // Is it on the $PATH?
  sCmd.vSprint("which %s 2>/dev/null"_Stz, szCompiler);

  if (Shell::iExecute(sCmd.szStz(), sStdOut) == 0) {
    return true;
  }

  // Can it be resolved as a relative or absolute path?
  sCmd.vSprint("ls %s 2>/dev/null"_Stz, szCompiler);
  if (Shell::iExecute(sCmd.szStz(), sStdOut) == 0) {
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
  s48K sStdOut;

  // Is it on the $PATH?
  sCmd.vSprint("scl enable %s 'which %s' 2>/dev/null || false"_Stz, szSclEnv, szCompiler);

  if (Shell::iExecute(sCmd.szStz(), sStdOut) == 0) {
    return true;
  }

  pLibInfo->m_eErrCode = EC_COMPILATION_FAILED;
  pLibInfo->m_sErrDetails.vSprint("Compiler %s not found or SCL environment %s does not exist"_Stz, szCompiler, szSclEnv);
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
/* Write the source string to a file as designated in pLibInfo                */
/*----------------------------------------------------------------------------*/
bool CompileLib::bWriteSrcToFile(ExtendedText* pSource, LibInfo* pLibInfo)
{
  ASSERT(pSource);
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

//  vTrace("~~pSource\n"_Stz);
//  pSource->vDump();
//  vTrace("~~~~~~~~~~~\n"_Stz);

  i8 nBlocks = pSource->nTextBlocks();
  for (i8 i_Block = 0; i_Block < nBlocks; i_Block++) {
    Stz *szSource = pSource->szTextBlock(i_Block);

//    if (nBlocks > 1) {
//      vTrace("~~szSource\n"_Stz);
//      vTrace("%s\n"_Stz,szSource);
//      vTrace("~~~~~~~~~~~\n"_Stz);
//    }

    if (!pSrcFile->bWriteBytes(szSource, szSource->nLength())) {
      pLibInfo->m_eErrCode = EC_COMPILATION_FAILED;

      strerror_r(pSrcFile->oSysCall().iError(), pLibInfo->m_sErrDetails.azChar(), pLibInfo->m_sErrDetails.nSpace());
      pLibInfo->m_sErrDetails.vCatSprint(" : %s"_Stz, pLibInfo->m_sSrcFilePath.szStz());

      pSrcFile->vClose();
      delete pSrcFile;
      return false;
    }
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
                                  C_GCC,
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
                                  C_CLANG,
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
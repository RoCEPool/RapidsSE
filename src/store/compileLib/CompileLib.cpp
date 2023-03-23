/*============================================================================*/
/* CompileLib.hpp - Dynamic code compiler and loader                          */
/*============================================================================*/

#include "BASE.h"
#include "CompileLib.hpp"
#include "LibInfo.hpp"
#include "eCB.h"
#include "Shell.hpp"

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
static i8  C_DEFAULT_COMPILER_VER = -1;

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

  return true;
}

// TODO: Provide a function to list the known compilers


/*----------------------------------------------------------------------------*/
/* Compiler source code to a dynamic library. Returns true on success.        */
/*----------------------------------------------------------------------------*/
bool CompileLib::bCompile(Stz* szSource, Stz* szDynLibNm, LibInfo* pLibInfo)
{
  ASSERT(szSource);
  ASSERT(szDynLibNm);
  ASSERT(pLibInfo);
  ASSERT(!pLibInfo->m_pvDynLib);
  pLibInfo->vClearErrors();

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
  vBuildGccCompileCommand(pLibInfo);

  // Execute the compile command
  if (!bCompileSrc(pLibInfo)) {
    return false;
  }

  // Check that the library exists
  ASSERT(File::bExists(pLibInfo->m_sDynLibPath.szStz()));

  return true;
}

/*----------------------------------------------------------------------------*/
/* Dynamically load a shared object library. Returns true on success.         */
/*----------------------------------------------------------------------------*/
bool CompileLib::bDynamicLink(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  pLibInfo->vClearErrors();

  return true;
}

/*----------------------------------------------------------------------------*/
/* Get the address of a symbol in the loaded library, or nullptr if error     */
/*----------------------------------------------------------------------------*/
void* CompileLib::pvSymbolAddr(Stz* szSymbolNm, LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  pLibInfo->vClearErrors();


}

/*----------------------------------------------------------------------------*/
/* Dynamically unload a shared object library. Returns true on success.       */
/*----------------------------------------------------------------------------*/
bool CompileLib::bDynamicUnlink(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  pLibInfo->vClearErrors();

  return true;
}

/*----------------------------------------------------------------------------*/
/* Delete a shared object library & associated files. Returns true on success.*/
/*----------------------------------------------------------------------------*/
void CompileLib::vDelete(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  pLibInfo->vClearErrors();

  ASSERT(pLibInfo->m_pvDynLib == nullptr);

  // Remove shared object, if exists
  if (!pLibInfo->m_sDynLibPath.bIsEmpty()) {
    File::bRemove(pLibInfo->m_sDynLibPath.szStz(), true);
  }

  // Remove log file, if exists
  if (!pLibInfo->m_sLogFilePath.bIsEmpty()) {
    File::bRemove(pLibInfo->m_sLogFilePath.szStz(), true);
  }

  // Remove source file, if exists
  if (!pLibInfo->m_sSrcFilePath.bIsEmpty()) {
    File::bRemove(pLibInfo->m_sSrcFilePath.szStz(), true);
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
"extern void vTraceExternal(char* psFormat, ...);\n"
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
  LibInfo* pLibInfo = new LibInfo();

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
    vTrace("Symbol resolution failed\n"_Stz);

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
  delete pLibInfo;
}


/*----------------------------------------------------------------------------*/
/*-- Private Functions--------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
void CompileLib::vBuildGccCompileCommand(LibInfo* pLibInfo, Stz* szGccPath)
{
  ASSERT(pLibInfo);
  ASSERT(!pLibInfo->m_sSrcFilePath.bIsEmpty());
  ASSERT(!pLibInfo->m_sDynLibPath.bIsEmpty());
  ASSERT(!pLibInfo->m_sLogFilePath.bIsEmpty());
  ASSERT(!pLibInfo->m_pvDynLib);

  pLibInfo->m_sCompileCmd.vReset();

  pLibInfo->m_sCompileCmd.vSprint("%s -std=c++11 -g -I../src -O3 -fpic -shared -o %s %s > %s 2>&1"_Stz,
                                  szGccPath,
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
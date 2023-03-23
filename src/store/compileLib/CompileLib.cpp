/*============================================================================*/
/* CompileLib.hpp - Dynamic code compiler and loader                          */
/*============================================================================*/

#include "BASE.h"
#include "CompileLib.hpp"
#include "LibInfo.hpp"
#include "eCB.h"

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
  pLibInfo->vClearErrors();

}

/*----------------------------------------------------------------------------*/
/* Dynamically load a shared object library. Returns true on success.         */
/*----------------------------------------------------------------------------*/
bool CompileLib::bDynamicLink(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  pLibInfo->vClearErrors();

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

}

/*----------------------------------------------------------------------------*/
/* Delete a shared object library & associated files. Returns true on success.*/
/*----------------------------------------------------------------------------*/
bool CompileLib::bDelete(LibInfo* pLibInfo)
{
  ASSERT(pLibInfo);
  pLibInfo->vClearErrors();

  ASSERT(pLibInfo->m_pvDynLib == nullptr);

  // Remove shared object, if exists
  if (!pLibInfo->m_sDynLibPath.bIsEmpty()) {
    File::bRemove(pLibInfo->m_sDynLibPath.szStz(), true);
  }

  // Remove object file, if exists
  if (!pLibInfo->m_sObjFilePath.bIsEmpty()) {
    File::bRemove(pLibInfo->m_sObjFilePath.szStz(), true);
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
"namespace BASE\n"
"{\n"
"  class TickingDevice\n"
"  {\n"
"  public:\n"
"    static extern i8 iGetTick(void);\n"
"  };\n"
"}\n"
"\n"
"void vTestEntry(void)\n"
"{\n"
"  printf(\"Inside vTestEntry!\\n\");\n"
"\n"
"  i8 iTick = TickingDevice::iGetTick();\n"
"  printtf(\"Got ticks = %lld\\n\", iTick);\n"
"  i8 iTick = TickingDevice::iGetTick();\n"
"  printtf(\"Got ticks = %lld\\n\", iTick);\n"
"  i8 iTick = TickingDevice::iGetTick();\n"
"  printtf(\"Got ticks = %lld\\n\", iTick);\n"
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

    if (!CompileLib::bDelete(pLibInfo)) {
      vTrace("Deletion failed\n"_Stz);
    }
    delete pLibInfo;
    return;
  }

  if (!bDynamicLink(pLibInfo)) {
    vTrace("Dynamic linking & loading failed\n"_Stz);

    if (!CompileLib::bDelete(pLibInfo)) {
      vTrace("Deletion failed\n"_Stz);
    }
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

    if (!CompileLib::bDelete(pLibInfo)) {
      vTrace("Deletion failed\n"_Stz);
    }
    delete pLibInfo;
    return;
  }

  // Call the function
  (*pfTestFn)();

  // Unlink
  if (!bDynamicUnlink(pLibInfo)) {
    vTrace("Dynamic unlinking failed\n"_Stz);

    if (!CompileLib::bDelete(pLibInfo)) {
      vTrace("Deletion failed\n"_Stz);
    }
    delete pLibInfo;
    return;
  }

  // Cleanup temporary files
  if (!CompileLib::bDelete(pLibInfo)) {
    vTrace("Deletion failed\n"_Stz);
  }
  delete pLibInfo;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
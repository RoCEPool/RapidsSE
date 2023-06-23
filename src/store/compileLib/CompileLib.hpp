/*============================================================================*/
/* CompileLib.hpp - Dynamic code compiler and loader                          */
/*============================================================================*/

#pragma once

namespace STORE
{
  class LibInfo;
  class ExtendedText;

  class CompileLib
  {
  private:
    CompileLib();   // No instantiating please, static only.

  public:
    static bool bInit();
    // TODO: list known compilers

    static bool  bCompile(Stz* szSource, Stz* szDynLibNm, LibInfo* pLibInfo, Stz* szCompiler = nullptr);
    static bool  bCompile(ExtendedText* pSource, Stz* szDynLibNm, LibInfo* pLibInfo, Stz* szCompiler = nullptr);
    static bool  bDynamicLink(LibInfo* pLibInfo);
    static void* pvSymbolAddr(Stz* szSymbolNm, LibInfo* pLibInfo);
    static bool  bDynamicUnlink(LibInfo* pLibInfo);
    static void  vDelete(LibInfo* pLibInfo);

    // Temporary
    static void  vTest(Stz* szCompiler = nullptr);

  private:
    static bool bValidateCompiler(Stz* szCompiler, LibInfo* pLibInfo);
    static bool bCheckCompilerExists(Stz* szCompiler, LibInfo* pLibInfo);
    static bool bCheckCompilerExists(Stz* szSclEnv, Stz* szCompiler, LibInfo* pLibInfo);
    static bool bWriteSrcToFile(Stz* szSource, LibInfo* pLibInfo);
    static bool bWriteSrcToFile(ExtendedText* pSource, LibInfo* pLibInfo);
    static void vBuildGccCompileCommand(LibInfo* pLibInfo);
    static void vBuildClangCompileCommand(LibInfo* pLibInfo);
    static void vBuildTccCompileCommand(LibInfo* pLibInfo);
    static void vBuildSclGccCompileCommand(LibInfo* pLibInfo);
    static void vBuildSclClangCompileCommand(LibInfo* pLibInfo);
    static bool bCompileSrc(LibInfo* pLibInfo);
  };
} // STORE

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

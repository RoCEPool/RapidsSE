/*============================================================================*/
/* CompileLib.hpp - Dynamic code compiler and loader                          */
/*============================================================================*/

#pragma once

namespace STORE
{
  class LibInfo;

  class CompileLib
  {
  private:
    CompileLib();   // No instantiating please, static only.

  public:
    static bool bInit();
    // TODO: list known compilers

    static bool  bCompile(Stz* szSource, Stz* szDynLibNm, LibInfo* pLibInfo);
    static bool  bDynamicLink(LibInfo* pLibInfo);
    static void* pvSymbolAddr(Stz* szSymbolNm, LibInfo* pLibInfo);
    static bool  bDynamicUnlink(LibInfo* pLibInfo);
    static void  vDelete(LibInfo* pLibInfo);

    // Temporary
    static void  vTest(void);

  private:
    static bool bWriteSrcToFile(Stz* szSource, LibInfo* pLibInfo);
    static void vBuildGccCompileCommand(LibInfo* pLibInfo, Stz* szGccPath = "g++"_Stz);
    static bool bCompileSrc(LibInfo* pLibInfo);
  };
} // STORE

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

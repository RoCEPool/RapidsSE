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
    static bool  bDelete(LibInfo* pLibInfo);
  };
} // STORE

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* TinyC.hpp -- Interface to Tiny C                                           */
/*============================================================================*/
#pragma once

#include "eAM.h"
#include "eEO.h"

#include "libtcc.h"

namespace STORE
{
  //----------------------------------------------------------------------------

  class TinyC
  {
  private:

  public:

    static void  vAddSymbol(void* pTccState, char* pSymbol, const void* fSymbol);
    static bool  bCompile(void* pTccState, char* pSrcCode);
    static void  vFinalize(void* pTccState);
    static void* pFindSymbol(void* pTccState, char* pSymbol);
    static void* pInitialize(void* pOpaque, void (void*, const char*));
    static bool  bRelocate(void* pTccState);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

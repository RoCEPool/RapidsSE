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

  class TinyC : public Object
  {
  private:
    TCCState* m_aTccState;

  // must be last
  Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    TinyC(void* pOpaque, void (void*, const char*));
    ~TinyC(void) noexcept(false);

    void  vAddSymbol(char* pSymbol, const void* fSymbol);
    bool  bCompile(char* pSrcCode);
    void* pFindSymbol(char* pSymbol);
    void* pInitialize();
    bool  bRelocate();

    Footer* aFooter(void);
  };

  TinyC* _pTinyC(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

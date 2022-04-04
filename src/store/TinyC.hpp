/*============================================================================*/
/* TinyC.hpp -- Interface to Tiny C                                           */
/*============================================================================*/
#pragma once

#include "eEO.h"
#include "eSR.h"

#include "libtcc.h"

namespace STORE
{
  //----------------------------------------------------------------------------

  struct DynamicSymbol
  {
    Stz* m_szSymbolNm;
    const void *m_afSymbol;
  };

  class TinyC : public Object
  {
  private:
    TCCState* m_aTccState;
  
    s32       m_sFunNm;
    Stz*      m_szSource;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    TinyC(void);
   ~TinyC(void) noexcept(false);

    static void vTinyError(void *aOpaque, const char *azMsg);

    static void vRepair(Object* pObject);
  
    void  vAddSymbol(char* azSymbol, const void* fSymbol);
    void  vAcquire(void* aOpaque, void (void*, const char*));
    void  vBegin();
    bool  bCompile(char* azSrcCode);
    void  vEnd();
    void* pFindSymbol(char* azSymbol);
    Stz*  szFunNm(void);
    void  vFunNm(Stz* szFunNm);
    void* pInitialize(void);
    bool  bRelocate(void*& aExeAddr);

    Footer* aFooter(void);
  };

  TinyC* _pTinyC(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

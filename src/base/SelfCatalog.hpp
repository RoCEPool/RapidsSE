/*============================================================================*/
/* SelfCatalog.hpp -- System-Level Self-Describing Function Catalog           */
/*============================================================================*/
#pragma once

namespace Resources
{
  class SelfCatalog
  {
  public:

    i8 m_nSelf;
    SelfFn* m_paSelfFn[CN_ENUMS];  // array of subsystem functions

  public:
    SelfCatalog(void);
  
    bool bCallFn(Stz* szSelfNm, void* aSelfArg);
  
    SelfFn* pFindFn(Stz* szSelfNm);
  
    void vListFn(void);
  
    void vAddFn(SelfFn* aSelfFn);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

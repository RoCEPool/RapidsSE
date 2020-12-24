/*============================================================================*/
/* SelfCatalog.hpp -- System-Self Function Catalog                            */
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
  
    bool bCallSelfFn(Stz* szSelfNm, void* aSelfArg);
  
    SelfFn* pFindSelfFn(Stz* szSelfNm);
  
    void vListSelftems(void);
  
    void vAddSelfFn(SelfFn* aSelfFn);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

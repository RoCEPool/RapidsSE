/*============================================================================*/
/* Drafter.hpp --                                                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Board.hpp"

namespace STORE
{
  class Drafter : public Object
  {
  private:
    Optimiser* m_pOptimiser;
    CardCtl*   m_pCardCtl;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Drafter(Optimiser* pOptimiser);
   ~Drafter(void) noexcept(false);

    static void vRepair(Object* pObject);

    bool bAdd     (TokenVec* pTknVec, eOA& reOptAct);

    bool bBelow   (TokenVec* pTknVec, eOA& reOptAct);

    bool bClear   (TokenVec* pTknVec, eOA& reOptAct);
    bool bCmp     (TokenVec* pTknVec, eOA& reOptAct);
    bool bCut     (TokenVec* pTknVec, eOA& reOptAct);

    bool bDepth   (TokenVec* pTknVec, eOA& reOptAct);
  
    bool bForm    (TokenVec* pTknVec, eOA& reOptAct);

    bool bHide    (TokenVec* pTknVec, eOA& reOptAct);

    bool bLoad    (TokenVec* pTknVec, eOA& reOptAct);

    bool bName    (TokenVec* pTknVec, eOA& reOptAct);

    bool bReprint (TokenVec* pTknVec, eOA& reOptAct);

    bool bSet     (TokenVec* pTknVec, eOA& reOptAct);
    bool bSub     (TokenVec* pTknVec, eOA& reOptAct);

    bool bWeave   (TokenVec* pTknVec, eOA& reOptAct);
    bool bWrite   (TokenVec* pTknVec, eOA& reOptAct);
  
  private:
    Footer* aFooter(void);
  };

  Drafter* _pDrafter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

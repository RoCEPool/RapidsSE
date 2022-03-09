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

    bool bAdd     (TokenVec* pTknVec, i8& riAction);

    bool bBelow   (TokenVec* pTknVec, i8& riAction);

    bool bClear   (TokenVec* pTknVec, i8& riAction);
    bool bCmp     (TokenVec* pTknVec, i8& riAction);
    bool bCut     (TokenVec* pTknVec, i8& riAction);

    bool bDepth   (TokenVec* pTknVec, i8& riAction);
  
    bool bForm    (TokenVec* pTknVec, i8& riAction);

    bool bHide    (TokenVec* pTknVec, i8& riAction);

    bool bLoad    (TokenVec* pTknVec, i8& riAction);

    bool bName    (TokenVec* pTknVec, i8& riAction);

    bool bReprint (TokenVec* pTknVec, i8& riAction);

    bool bSet     (TokenVec* pTknVec, i8& riAction);
    bool bSub     (TokenVec* pTknVec, i8& riAction);

    bool bWeave   (TokenVec* pTknVec, i8& riAction);
    bool bWrite   (TokenVec* pTknVec, i8& riAction);
  
  private:
    Footer* aFooter(void);
  };

  Drafter* _pDrafter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Drafter.hpp --                                                               */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

namespace STORE
{
  class Drafter : public OptimiserTool
  {
  private:
    Optimiser* m_pOptimiser;
    CardCtl*   m_pCardCtl;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Drafter(Bench* pBench);
   ~Drafter(void) noexcept(false);

    static void vRepair(Object* pObject);

    bool bClear   (TokenVec* pTknVec, eOA& reOptAct);
    bool bCmp     (TokenVec* pTknVec, eOA& reOptAct);
    bool bCut     (TokenVec* pTknVec, eOA& reOptAct);

    bool bLoad    (TokenVec* pTknVec, eOA& reOptAct);

    bool bName    (TokenVec* pTknVec, eOA& reOptAct);

    bool bReprint (TokenVec* pTknVec, eOA& reOptAct);

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

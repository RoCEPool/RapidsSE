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

    bool bWeave   (TokenVec* pTknVec, eOA& reOptAct);

  private:
    Footer* aFooter(void);
  };

  Drafter* _pDrafter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

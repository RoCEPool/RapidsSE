/*============================================================================*/
/* Optimiser.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Bench.hpp"
#include "CardCtl.hpp"
#include "Dissect.hpp"
#include "Rules.hpp"
#include "Weave.hpp"

namespace STORE
{
  class Optimiser : public Object
  {
  private:
    i8 m_iOptimiserLvl;
    PipelineSpec* m_pUnOptPipelineSpec;
  
    Bench*   m_pBench;
    CardCtl* m_pCardCtl;
    Dissect* m_pDissect;
    Rules*   m_pRules;
    Weave*   m_pWeave;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Optimiser(PipelineSpec* pUnOptPipelineSpec, i8 iOptimiserLvl);
   ~Optimiser(void) noexcept(false);

    static void vRepair(Object* pObject);

    CardCtl* pCardCtl(void);
  
    void vOptimise(void);

    Stz* szWeave(void);

  private:

    void vOptimise_Board(void);

    Footer* aFooter(void);
  };

  Optimiser* _pOptimiser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

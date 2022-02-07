/*============================================================================*/
/* Optimiser.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Bench.hpp"
#include "Dissect.hpp"
#include "Tools.hpp"
#include "Weave.hpp"

namespace STORE
{
  class Optimiser : public Object
                  , public ListExo2_t<Board>
  {
  private:
    PipelineSpec* m_pPipelineSpec_original;
  
    Bench*   m_pBench;
    Dissect* m_pDissect;
    Tools*   m_pTools;
    Weave*   m_pWeave;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Optimiser(PipelineSpec* pPipelineSpec_original);
   ~Optimiser(void) noexcept(false);

    static void vRepair(Object* pObject);

    PipelineSpec* pPipelineSpec_original(void);
    void vRun(i8 iOptimiserLvl);

 

  private:

    Footer* aFooter(void);
  };

  Optimiser* _pOptimiser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

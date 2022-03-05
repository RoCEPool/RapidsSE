/*============================================================================*/
/* Optimiser.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Bench.hpp"
#include "CardCtl.hpp"
#include "Dissect.hpp"
#include "Weave.hpp"

namespace STORE
{
  class Optimiser : public Object
  {
  private:
    TagSet* m_p_JobTags;
  
    PipelineSpec* m_pUnOptPipelineSpec;
  
    CardCtl* m_pCardCtl;
  
    Bench*   m_pBench;
    Dissect* m_pDissect;
    Weave*   m_pWeave;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Optimiser(TagSet* p_JobTags);
  
   ~Optimiser(void) noexcept(false);

    static void vRepair(Object* pObject);

    Board* pBoard(void);
  
    CardCtl* pCardCtl(void);
  
    void vDissect(PipelineSpec* pUnOptPipelineSpec);
   
    TagSet* pJobTags(void);
  
    void vOptimise(void);
  
    void vPipelineSpec(PipelineSpec* pUnOptPipelineSpec);

    void vToPipelineSpec(PipelineSpec* pPipelineSpec);
  
    Stz* szWeave(void);

  private:


    Footer* aFooter(void);
  };

  Optimiser* _pOptimiser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Optimiser.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Bench.hpp"
#include "CardCtl.hpp"
#include "Dissect.hpp"
#include "Drafter.hpp"
#include "Weave.hpp"

namespace STORE
{
  class Optimiser : public Object
  {
  private:
    TagSet* m_p_JobTags;
  
    PipeSpec* m_pUnOptPipeSpec;
  
    CardCtl* m_pCardCtl;
  
    Bench*   m_pBench;
    Dissect* m_pDissect;
    Drafter* m_pDrafter;
    Weave*   m_pWeave;
  
    Card_SubSet* m_pPaste;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Optimiser(TagSet* p_JobTags);
  
   ~Optimiser(void) noexcept(false);

    static void vRepair(Object* pObject);

    CardCtl* pCardCtl(void);
  
    void vDissect(PipeSpec* pm_pUnOptPipeSpec);
  
    Drafter* pDrafter(void);

    Board* pHeadBoard(void);

    TagSet* pJobTags(void);
  
    void vOptimise(void);
  
    Card_SubSet* pPaste(void);
    void vPipeSpec(PipeSpec* pUnOptPlineSpec);
    void vPopBoard(void);
    void vPushBoard(Board* pBoard);
 
    void vToPipeSpec(PipeSpec* pPipeSpec);
  
    Stz* szWeave(void);

  private:


    Footer* aFooter(void);
  };

  Optimiser* _pOptimiser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

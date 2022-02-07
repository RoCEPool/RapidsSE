/*============================================================================*/
/* Optimiser.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Dissect.hpp"
#include "Tools.hpp"

namespace STORE
{
  class Optimiser : public Object
                  , public ListExo2_t<Board>
  {
  private:
    PipelineSpec* m_pPipelineSpec_original;
  
    Dissect* m_pDissect;
    Tools*   m_pTools;

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

  
  
  
    void vWalkUnits(std::function<void(Card* pCard)> f_vClosure);
    void vWalkAllCards(std::function<void(Card* pCard)> f_vClosure);

    void vExpand(void);
    void vOrdinals(void);
    void vRenumber(void);
    void vReproject(void);
    void vRework(void);
    void vStreamOrder(void);
    void vSynthesize(void);

 

  private:

    Footer* aFooter(void);
  };

  Optimiser* _pOptimiser(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

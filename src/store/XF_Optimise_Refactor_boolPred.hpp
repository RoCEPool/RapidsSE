/*============================================================================*/
/* XF_Optimise_Refactor_boolPred.hpp -- Refactor boolPred                     */
/*============================================================================*/
#pragma once

#include "XF_OPTIMISE.hpp"

namespace STORE
{
  class XF_Optimise_Refactor_boolPred : public XF_OPTIMISE
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_Refactor_boolPred(CardIter* pCI);
   ~XF_Optimise_Refactor_boolPred(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    bool bOptimise(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_Refactor_boolPred* _pXF_Optimise_Refactor_boolPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

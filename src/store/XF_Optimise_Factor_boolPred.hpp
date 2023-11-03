/*============================================================================*/
/* XF_Optimise_Factor_boolPred.hpp -- Refactor boolPred                     */
/*============================================================================*/
#pragma once

#include "XF_OPTIMISE.hpp"

namespace STORE
{
  class XF_Optimise_Factor_boolPred : public XF_OPTIMISE
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Optimise_Factor_boolPred(CardIter* pCI);
   ~XF_Optimise_Factor_boolPred(void) noexcept(false);

    static void vOptimise(CardIter* pCI);

    bool bOptimise(void);

    void vFactor_boolPred(void);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_Factor_boolPred* _pXF_Optimise_Factor_boolPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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

    void vFactor_andConj(Card_AndConj* p_andConj);
    void vFactor_notConj(Card_NotConj* p_notConj);
    void vFactor_orConj (Card_OrConj*  p_orConj);
    void vFactor_rlnConj(Card_RlnConj* p_rlnConj);

    void vRotate_a(void);
    void vRotate_b(void);
  private:
    Footer* aFooter(void);
  };

  XF_Optimise_Factor_boolPred* _pXF_Optimise_Factor_boolPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

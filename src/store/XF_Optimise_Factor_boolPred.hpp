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

    i8   nFactor_boolPred(Card_BoolPred* p_boolPred);

    i8   nFactor_andConj(Card_AndConj* p_andConj);
    i8   nFactor_notConj(Card_NotConj* p_notConj);
    i8   nFactor_orConj (Card_OrConj*  p_orConj);
    i8   nFactor_rlnConj(Card_RlnConj* p_rlnConj);

    void vRotate_Or_2ar3(Card_CONJSUBROOT* p_CONJSUBROOT);
    void vRotate_Or_2arm(Card_CONJSUBROOT* p_CONJSUBROOT);

    bool bTest_Rotate_Or_2ar3(Card_CONJSUBROOT* p_CONJSUBROOT);
    bool bTest_Rotate_Or_2arm(Card_CONJSUBROOT* p_CONJSUBROOT);

  private:
    Footer* aFooter(void);
  };

  XF_Optimise_Factor_boolPred* _pXF_Optimise_Factor_boolPred(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

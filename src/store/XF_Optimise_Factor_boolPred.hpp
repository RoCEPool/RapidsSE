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

    void vFactor_boolPred(Card_BoolPred* p_boolPred);

    eRC eFactor_boolPred(Card_BoolPred* p_boolPred);
    eRC eFactor_andConj(Card_AndConj* p_andConj);
    eRC eFactor_notConj(Card_NotConj* p_notConj);
    eRC eFactor_orConj (Card_OrConj*  p_orConj);
    eRC eFactor_rlnConj(Card_RlnConj* p_rlnConj);

    eRC eRotate_Or_2ar3(Card_CONJSUBROOT* p_CONJSUBROOT);
    eRC eRotate_Or_2arm(Card_CONJSUBROOT* p_CONJSUBROOT);

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

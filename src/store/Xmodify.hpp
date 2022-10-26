/*============================================================================*/
/* Xmodify.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xcreate.hpp"

namespace STORE
{
  class Table;

  class Xmodify : public Xcreate
  {
  public:

    void T_vPRED_CullByKids(Card_PRED* p_PRED);


    void T_vCmpPred_MarkBound(Card_CmpPred* p_cmpPred);

    bool X_bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark);
    void X_vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark);
    void X_vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred);

    void X_vFilter_CullPred(Card_Filter* p_filter, char cWithMark);

    // must be last
    Footer m_xFooter;

  public:
    Xmodify(eCN eClass);
   ~Xmodify(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

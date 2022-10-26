/*============================================================================*/
/* Xtest.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xglean.hpp"

namespace STORE
{
  class Table;

  class Xtest : public Xglean
  {
  public:

    bool T_bColumnRef_IsColumnRef(Card_ColumnRef* p_columnRef);

    bool T_bTerm_IsIndexColOrdinal(Card_Term* p_term);
    bool T_bTerm_IsParamOrdinal(Card_Term* p_term);
    bool T_bTerm_IsStreamColOrdinal(Card_Term* p_term);
    bool T_bTerm_IsTableColOrdinal(Card_Term* p_term);

    bool T_bOrPred_HasBounds(Card_OrPred* p_orPred, Index* pIndex);
    bool T_bAndPred_HasBounds(Card_AndPred* p_andPred, Index* pIndex);
    bool T_bCmpPred_HasBounds(Card_CmpPred* p_cmpPred, Index* pIndex);

    bool T_bPRED_HasBounds (Card_PRED* p_PRED, Index* pIndex);
    static bool T_bPRED_IsPred(Card* pArgCard);

    bool X_bFilter_IsEmptyFilter(Card_Filter* p_filter);

    // must be last
    Footer m_xFooter;

  public:
    Xtest(eCN eClass);
   ~Xtest(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

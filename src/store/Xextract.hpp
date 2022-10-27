/*============================================================================*/
/* Xextract.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xaccess.hpp"

namespace STORE
{
  class Table;

  class Xextract : public Xaccess
  {
  public:

    i8   G_iColumnRef_ColumnOrd(Card_ColumnRef* p_columnRef);
    char G_cColumnRef_TableSyn(Card_ColumnRef* p_columnRef);
    char G_cColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef);
    void G_vColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef, String* psTableSyn);

    ePB  G_eTerm_Bound(Card_Term* p_term);
    i8   G_iTerm_IndexColNo(Card_Term* p_term);
    eRO  G_eTerm_RelOp(Card_Term* p_term);
    i8   G_iTerm_StreamColNo(Card_Term* p_term);
    i8   G_iTerm_TableColNo(Card_Term* p_term);
    char G_cTerm_TableSyn(Card_Term* p_term);
    eTE  G_eTerm_TermEval(Card_Term* p_term);

    i8   X_iCandIndex_Rank(Card_CandIndex* p_candIndex);
    void X_vCandIndex_Rank(Card_CandIndex* p_candIndex, i8 iRank);


    // must be last
    Footer m_xFooter;

  public:
    Xextract(eCN eClass);
   ~Xextract(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

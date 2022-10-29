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

    i8   E_iCandIndex_Rank(Card_CandIndex* p_candIndex);
    void E_vCandIndex_Rank(Card_CandIndex* p_candIndex, i8 iRank);

    i8   E_iColumnRef_ColumnOrd(Card_ColumnRef* p_columnRef);
    char E_cColumnRef_TableSyn(Card_ColumnRef* p_columnRef);
    char E_cColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef);
    void E_vColumnRef_TableSyn_x(Card_ColumnRef* p_columnRef, String* psTableSyn);

    i8   E_iStream_OrdLo(Card_Stream* p_stream);
    i8   E_iStream_OrdHi(Card_Stream* p_stream);

    ePB  E_eTerm_Bound(Card_Term* p_term);
    i8   E_iTerm_IndexColNo(Card_Term* p_term);
    eRO  E_eTerm_RelOp(Card_Term* p_term);
    i8   E_iTerm_StreamColOdn(Card_Term* p_term);
    i8   E_iTerm_TableColNo(Card_Term* p_term);
    char E_cTerm_TableSyn(Card_Term* p_term);
    eTE  E_eTerm_TermEval(Card_Term* p_term);


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

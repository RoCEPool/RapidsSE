/*============================================================================*/
/* Card_Bound.hpp -- Abstract Bound                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Bound : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Bound(void);
    Card_Bound(Stz* szRelnOp);
   ~Card_Bound(void) noexcept(false);

    static Card_Bound*    pCreateFrom_PRED(Card_PRED* p_PRED,
                                           char cTblSyn_boundedTo);
    static Card_Bound*    pBound_New(Stz* szRelnOp);

    eXK eStageCast(Card_TablePtn* p_tablePtn_prev,
                   Card_TablePtn* p_tablePtn_curr,
                   Card_Index*    p_index_curr,
                   Card_Bound*    p_bound_curr,
                   Card_Term*&    rp_term_tblCol_to_be_hashed);

    Card_Term* pGetTerm(i8 i_Term);

    bool bIsEBound(void);
    bool bIsLBound(void);
    bool bIsUBound(void);

    Card_Term* pGet_idxColOdn(void);
    Stz* szGet_idxColOdn(void);

    Card_Term* pGet_tblColOdn(void);
    Stz* szGet_tblColOdn(void);

    void vFlipTerms(Card_Projection* p_projection_reduced);
  };

  Card_Bound* _pCard_Bound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

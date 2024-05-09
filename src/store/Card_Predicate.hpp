/*============================================================================*/
/* Card_Predicate.hpp -- Boolean Predicate                                    */
/*============================================================================*/
#pragma once

#include "Card_CONJSUBROOT.hpp"
#include "Card_Correlation.hpp"

namespace STORE
{
  class Card_Predicate : public Card_CONJSUBROOT
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Predicate(void);
   ~Card_Predicate(void) noexcept(false);

    static Card_Predicate* pTo_boolPred(CardIter* pCI, Card_Expr* p_expr_arg);

    void vAppend_and_rlnConj(Card_RlnConj *p_rlnConj);
    void vApplyCast(CardIter* pCI, Card_Emit* p_Dic_expr);
    void vBump_expCol(i8 bBumpBy=1);
    bool bContains(Symbol* pGivenSymbol);
    void vCopyMarked_rlnConj_To_table_filter(CardIter* pCI, Stz* szWithMark);
    void vUpdate_expCol(CardIter* pCI, Card_Emit* p_Dic_expr, bool bAlt=false);
    i8   nCullMarked_rlnConj(Stz* szWithMark);
    bool bCullMarked_rlnConj(Card_CONJ* p_conj, Stz* szWithMark);
    void vDeleteFacetIf(eSF eFacet, eSF eIfFacet, char cTblSyn);
    bool bHas_term(Symbol* pWithSymbol);
    bool bIsBraced(void);
    bool bIsCompound(void);
    bool bIsLiterals(void);
    bool bIsScalarCol(void);
    bool bIsScalarKey(void);
    void vMark_rlnConj_Wrt_table(CardIter* pCI, Stz* szWithMark);
    void vOntoRpnExpr(String* psRpnExpr);
    void vReduce_not(CardIter* pCI);
    void vRemove_rlnConj(Card_RlnConj *p_rlnConj);
    Card_CONJ* pRoot_conj(void);
     void vX_expCol_To_tblCol(CardIter* pCI);
    void vX_tblCol_To_expCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Predicate* _pCard_Predicate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

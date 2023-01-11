/*============================================================================*/
/* Card_Table.hpp -- Table                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Scope.hpp"

namespace STORE
{
  class Card_Table : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Table(void);
    Card_Table(Stz* szTableNm);
   ~Card_Table(void) noexcept(false);

    static Card_Table* pResolve_table_By_tblSyn(CardIter* pCI, char cTblSyn, i8 i_Before);

    void vAddPredToTableFilter(CardIter* pCI, Card_PRED* p_PRED);

    void vBounds_AddPred(Card_Index* p_index,
                           Card_Bounds* p_bounds, Card_PRED *p_PRED);
    void vBounds_DeriveFrom_filter(Card_Filter* p_filter,
                                   bool bParamAllowedInBounds);
    void vComputeProjection(CardIter* pCI); // prototype
    bool bScopeContains_Pred(Card_PRED* p_pred, Scope* pScope);
    bool bScopeContains_term(Card_Term* p_term, Scope* pScope);
    void vSet_Syn(char* cTableSyn);
  };

  Card_Table* _pCard_Table(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

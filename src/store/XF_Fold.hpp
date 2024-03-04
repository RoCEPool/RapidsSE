/*============================================================================*/
/* XF_Fold.hpp -- XF_Fold                                                           */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class CarryForward;

  class XF_Fold : public Object
  {
  private:

    i8         m_i_depth;
    Card_Deck* m_p_deck;

    Card_Correlation* m_p_correlation;

    SubQuery*  m_pSubQuery;
    SubQuery*  m_pOuterQuery;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Fold(i8 i_depth);
    XF_Fold(i8 i_depth, Card_Deck* p_deck);
   ~XF_Fold(void) noexcept(false);

    Card_Correlation* p_correlation(void);
    void v_correlation(Card_Correlation* p_correlation);

    Card_Deck* p_deck(void);
    void v_deck(Card_Deck* p_deck);

    i8 i_depth(void);

    SubQuery* pOuterQuery(void);
    void vOuterQuery(SubQuery* pOuterQuery);

    SubQuery* pSubQuery(void);
    void vSubQuery(SubQuery* pSubQuery);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


    void vDerive_Correleted_fold_projection(CardIter* pCI);

    void vFold_basic(CardIter* pCI, Stz* szFoldNm, eCK eCardWithSQ);

    CarryForward* pFold_from(Stz* szFoldNm, CardIter* pCI);

    CarryForward* pFold_nc_subQuery(CardIter* pCI, Stz* szFoldNm,
                                    eCK eCardWithSQ, eQO eWhatOp);

    i8 nTables_In_SubQuery(CardIter* pCI, Stz* szFold, eCK eCardWithSQ);

  protected:
    Footer* aFooter(void);
  };

  XF_Fold* _pXF_Fold(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

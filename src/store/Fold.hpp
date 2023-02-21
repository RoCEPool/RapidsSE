/*============================================================================*/
/* Fold.hpp -- Fold                                                           */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class CarryForward;

  class Fold : public Object
  {
  private:

    i8         m_i_depth;
    Card_Deck* m_p_deck;

    Card_Correlation* m_p_correlation;

    SubQuery*  m_pSubQuery;
    Card_Fold* m_p_fold;
    SubQuery*  m_pOuterQuery;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Fold(i8 i_depth);
    Fold(i8 i_depth, Card_Deck* p_deck);
   ~Fold(void) noexcept(false);

    Card_Correlation* p_correlation(void);
    void v_correlation(Card_Correlation* p_correlation);

    Card_Deck* p_deck(void);
    void v_deck(Card_Deck* p_deck);

    i8 i_depth(void);

    Card_Fold* p_fold(void);
    void v_fold(Card_Fold* p_fold);

    SubQuery* pOuterQuery(void);
    void vOuterQuery(SubQuery* pOuterQuery);

    SubQuery* pSubQuery(void);
    void vSubQuery(SubQuery* pSubQuery);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    CarryForward* pFlatten_74_H_A_NC_ro(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    CarryForward* pFold_exists(Stz* szFold, CardIter* pCI);

    CarryForward* pFold_from(Stz* szFold, CardIter* pCI);

    CarryForward* pFold_nc_subQuery(Stz* szFold, eCK eCardWithSQ, CardIter* pCI);

  protected:
    Footer* aFooter(void);
  };

  Fold* _pFold(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

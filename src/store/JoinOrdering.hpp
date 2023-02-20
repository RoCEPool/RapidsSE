/*============================================================================*/
/* JoinOrdering.hpp -- Join Ordering                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class JoinOrdering : public Object
  {
  private:

    i8         m_i_depth;
    Card_Deck* m_p_deck;

    SubQuery*  m_pSubQuery;
    Card_JoinOrdering* m_p_JoinOrdering;
    SubQuery*  m_pOuterQuery;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    JoinOrdering(i8 i_depth);
    JoinOrdering(i8 i_depth, Card_Deck* p_deck);
   ~JoinOrdering(void) noexcept(false);

    Card_Deck* p_deck(void);
    void v_deck(Card_Deck* p_deck);

    i8 i_depth(void);

    Card_JoinOrdering* p_JoinOrdering(void);
    void v_JoinOrdering(Card_JoinOrdering* p_JoinOrdering);

    SubQuery* pOuterQuery(void);
    void vOuterQuery(SubQuery* pOuterQuery);

    SubQuery* pSubQuery(void);
    void vSubQuery(SubQuery* pSubQuery);

  protected:
    Footer* aFooter(void);
  };

  JoinOrdering* _pJoinOrdering(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

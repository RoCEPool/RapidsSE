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

    Card_Deck* m_p_deck;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    JoinOrdering(Card_Deck* p_deck);
   ~JoinOrdering(void) noexcept(false);

    Card_Deck* p_deck(void);
    void v_deck(Card_Deck* p_deck);

  protected:
    Footer* aFooter(void);
  };

  JoinOrdering* _pJoinOrdering(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* CardIter.hpp --                                                            */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class CardIter : public Object
  {
  protected:
    Card*  m_pCard;
    i8     m_i_Child;
    Card*  m_pCurrChild;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardIter(Card* pCard);
    CardIter(eCN eClass, Card* pCard);
   ~CardIter(void) noexcept(false);


    Card* pEachChild(void);
    Card* pEachChild(i8& ri_Child);
    Card* pEachChild(eCK eCard);
    Card* pEachChild(eCK eCard, i8& ri_Main);
    Card* pEachChild(i8& ri_Main, eCK& reCard);
  private:

    Footer* aFooter(void);
  };

  CardIter* _pCardIter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
    Card*  m_pChild;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardIter(Card* pCard);
    CardIter(eCN eClass, Card* pCard);
   ~CardIter(void) noexcept(false);

    i8 i_Child(void);

    Card* pChild(void);

    bool  bEachChild(void);
    Card* pEachChild(void);
    Card* pEachChild(i8& ri_Child);
    Card* pEachChild(eCK eCard);
    Card* pEachChild(eCK eCard, i8& ri_Main);
    Card* pEachChild(i8& ri_Main, eCK& reCard);

    void vForward(void);

    Card* pGetChild(i8 i_Child);

    bool bLess(void);

    bool bMore(void);

    Card* pNextChild(void);
    Card* pNextChildIf(eCK eCard);

    Card* pPrevChild(void);
    Card* pPrevChildIf(eCK eCard);

    void vRemoveChild(Card* pChild);
    void vRemoveCurr(eCK eCrossCheck);
    void vRemoveDeleteChild(Card*& rpChild);
    void vRemoveDeleteCurr(eCK eCrossCheck);
    void vRestart(void);
    void vRewind(void);

    void vSet(Card* pChild);
    void vSet(i8 iChild, Card* pChild);

  private:

    Footer* aFooter(void);
  };

  CardIter* _pCardIter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

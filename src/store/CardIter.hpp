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

    bool   m_bDontDescend;

    bool   m_bActive;

    ListExo2_t<CardIter>* m_pRecurseStack;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    CardIter(void);
    CardIter(Card* pCard);
    CardIter(CardIter* pCardIter_src);
    CardIter(eCN eClass, Card* pCard);
   ~CardIter(void) noexcept(false);

    void vChangeCard(Card* pCard);
    i8 i_Child(void);
    Card* pChild(void);

    void vDontDescend(void);

    bool  bEachChild(void);
    Card* pEachChild(void);
    Card* pEachChild(i8& ri_Child);
    Card* pEachChild(eCK eCard);
    Card* pEachChild(eCK eCard, i8& ri_Main);
    Card* pEachChild(i8& ri_Main, eCK& reCard);
    Card* pEachChild_reverse(eCK eCard);

    void vForward(void);

    Card* pGetChild(i8 i_Child);

    bool bJustRestarted(void);

    bool bLess(void);

    bool bMore(void);

    Card* pNextChild(void);
    Card* pNextChildIf(eCK eCard);

    Card* pPrevChild(void);
    Card* pPrevChildIf(eCK eCard);

    Card* pRecurseEach(void);
    Card* pRecurseEach(eCK eCard);
    void vRemoveChild(Card* pChild);
    void vRemoveCurr(eCK eCrossCheck);
    void vRemoveDeleteChild(Card*& rpChild);
    void vRemoveDeleteCurr(eCK eCrossCheck);
    void vRestart(void);
    void vRewind(void);
    Card* pRewindChild(void);

    void vSet(Card* pChild);
    void vSet(CardIter* pCardIter_src);
    void vSet(i8 iChild, Card* pChild);

  protected:

    bool bPopRecurseIter(void);
    void vPushRecurseIter(void);

  private:

    Footer* aFooter(void);
  };

  CardIter* _pCardIter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* ChildCards.hpp -- Card Set                                                    */
/*============================================================================*/
#pragma once

#include "ListExo2_t.hpp"

#include "eCK.h"
#include "eSF.h"

#include "StzList.hpp"

namespace STORE
{
  class Card;
  class Card_Table;
  class Symbol;

  class ChildCards : public ListExo2_t<Card>
  {
  public:
    Card* m_pCard;

    ChildCards(Card* pCard);
   ~ChildCards(void) noexcept(false);

    void vAppendChild(Card* pChild, Card* pParent=nullptr);

    i8   nChildCards(void);
    i8   nChildCards(eCK eChild_arg);

    void vDeleteAllChildren(void);
    void vDeleteChild(Card* pChild);
    void vDeleteSub(Card* pSub);

    void vDiscardHead(void);

    void vEnsureFirst(eCK eChild);
    void vEnsureLast(eCK eChild);

    i8    iFindChild(Card* pChild);
    Card* pFindChild(eCK eCard);
    Card* pFindChild(eCK eCard, i8& i_Child);
    Card* pFindChild(eCK eCard, Stz* szFacet, i8& i_Child);
    Card* pFindChildWithSub(eCK eChild, eCK eSub, Stz* szFacet, i8& i_Child);
    Card* pFindSubCard(eCK eSub);
    Card_Table* pFind_table(char cTableSyn, Card*& rpMainCard);

    eCK   eGetChild(i8 i_Child, Card*& rpChild);
    Card* pGetChild(i8 i_Child);
    Card* pGetChild(i8 i_Child, eCK& reChild);

    bool  bHeadSame(eSF eFacet, Stz* szFacet);

    void  vInsertChildAfter(Card* pChild, Card* pAfter);
    void  vInsertChildBefore(Card* pChild, Card* pBefore);

    void  vMorphChildrenTo(eCK eFrom, eCK eTo);

    eCK   ePeekChild(i8 i_Child=0);
    Card* pPeekChild(i8 i_Child=0);
    Card* pPeekChild_tail(void);
    Card* pPopChild(void);
    Card* pPopChild_tail(void);
    bool  bPopDeleteChildIf(eCK eCard, Symbol xSymbol);
    void  vPushChild(Card* pChild);

    bool bRemoveChild(Card* pChild);
    void vRemoveChild(Card* pChild);
    void vRemoveChildLeavingSubs(Card* pChild);
    void vRemoveSub(Card* pSub);
    void vReplaceChild(Card* pChild, Card* pWith);
    void vRotateFirst(Card* pChild);
    void vRotateLast(Card* pChild);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

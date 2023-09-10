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
    ChildCards(void);
   ~ChildCards(void) noexcept(false);

    void vAppendChild(Card* pChild);

    i8   nChildCards(void);

    void vDeleteAllChildren(void);
    bool bDeleteChild(Card* pChild);
    void vDeleteChild(Card* pChild);
    void vDeleteSub(Card* pSub);

//    void vEnsureAfter(eCK eChild, eCK eChild_after);
//    void vEnsureBefore(eCK eChild, eCK eChild_before);
    void vEnsureFirst(eCK eChild);
    void vEnsureLast(eCK eChild);

    i8    iFindChild(Card* pChild);
    Card* pFindChild(eCK eCard);
    Card* pFindChild(eCK eCard, i8& i_Child);
    Card* pFindChild(eCK eCard, Stz* szValue, i8& i_Child);
    Card* pFindChildWithSub(eCK eChild, eCK eSub, Stz* szValue, i8& i_Child);
    Card* pFindSubCard(eCK eCard);
    Card_Table* pFindTableCard(char cTableSyn, Card*& rpMainCard);

    Card* pGetChild(i8 i_Child);
    Card* pGetChild(i8 i_Child, eCK& reCard);
    Card* pGetChild_bySym(Symbol* pSymbol);

    void  vInsertChildAfter(Card* pChild, Card* pAfter);
    void  vInsertChildBefore(Card* pChild, Card* pBefore);

    void vMorphChildrenTo(eCK eFrom, eCK eTo);

    void vOntoStzList(StzList* pStzList);

    Card* pPeekChild(void);
    Card* pPeekChild_last(void);
    Card* pPopChild(void);
    Card* pPopChild_tail(void);
    Card* pPopChildIf(Symbol xSymbol);
    bool  bPopDeleteChildIf(eCK eCard, Symbol xSymbol);
    void  vPushChild(Card* pChild);

    bool bRemoveChild(Card* pChild);
    void vRemoveChild(Card* pChild);
    void vRemoveSub(Card* pSubCard);
    void vReplaceChildWithItsSubs(Card* pChild);
    void vReplaceChild(Card* pChild, Card* pWith);
    void vRotateFirst(Card* pChild);
    void vRotateLast(Card* pChild);

    void vTransferChildren(Card* pFrom);
    void vTransferChildren(Card* pFrom, eSF eSymbol_limit);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

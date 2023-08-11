/*============================================================================*/
/* ChildCards.hpp -- Card Set                                                    */
/*============================================================================*/
#pragma once

#include "ListExo2_t.hpp"

#include "eCK.h"
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

    i8   nChildCards(void);
    void vConcatChildren(Stz* szTerms, eCK eChild=CK_NONE);

    i8    iFindChild(eCK eCard);
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

    void vMorphChildrenTo(eCK eFrom, eCK eTo);

    void vOntoStzList(StzList* pStzList);

    Card* pPeekChild(void);
    Card* pPopChild(void);
    Card* pPopChildIf(Symbol xSymbol);
    bool  bPopDeleteChildIf(eCK eCard, Symbol xSymbol);
    Card* pPlopChild(void);

    void vRemoveChild(Card* pChildCard);
    void vDeleteAllChildren(void);
    void vDeleteChild(Card* pChild);
    void vRemoveSub(Card* pSubCard);
    void vReplaceChildWithItsSubs(Card* pChildCard);
    void vReplaceChild(Card* pChildCard, Card* pWithCard);
    void vRotateChild(Card* pChildCard);

    void vTransferChildren(Card* pFrom);
    void vTransferChildren_limited(Card* pFrom, Symbol xSymbol);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

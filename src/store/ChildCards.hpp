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

    void vMorphChildrenTo(eCK eFrom, eCK eTo);

    void vOntoStzList(StzList* pStzList);

    Card* pPopChild(void);
    Card* pPlopChild(void);

    void vPromoteSubs(Card* pChildCard);

    void vRemoveAllChildren(void);
    void vRemoveChild(Card* pChildCard);
    void vDeleteAllChildren(void);
    void vDeleteChild(Card* pChild);
    void vRemoveSub(Card* pSubCard);
    void vReplaceChildWithItsSubs(Card* pChildCard);
    void vReplaceChild(Card* pChildCard, Card* pWithCard);
    void vRotateChild(Card* pChildCard);

    StzList* pToStzList(void);

    void vTransferChildren(Card* pFromCard);
    void vTransferFrom(Card* pCards, Card* pFrom,
                       Card* pAt=nullptr, bool bAfter=true);
    void vTransferSubset(Card* pCards, Card* pFrom, Card* pTill,
                         Card* pAt=nullptr, bool bAfter=true);
    void vTransferTill(Card* pCards, Card* pTill,
                       Card* pAt=nullptr, bool bAfter=true);

    i8 nTrimChildren(eCK eCard, Stz* szSymbol);


    void vValidate_mayHave(eCK eCard, ...);
    eCK  eValidate_mustEither(eCK eCard, ...);
    //void vValidate_mustHave(eCK eCard, ...);
    void vValidate_notHave(eCK eCard, ...);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

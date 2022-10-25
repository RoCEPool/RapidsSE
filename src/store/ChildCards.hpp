/*============================================================================*/
/* ChildCards.hpp -- Card Set                                                    */
/*============================================================================*/
#pragma once

#include "ListExo2_t.hpp"

#include "eCK.h"

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

    i8    iFindChild(Card* pChild);
    Card* pFindChild(eCK eCard);
    Card* pFindChild(eCK eCard, i8& i_Child);
    Card* pFindChild(eCK eCard, Stz* szValue, i8& i_Child);
    Card* pFindChildWithSub(eCK eChild, eCK eSub, Stz* szValue, i8& i_Child);
    Card* pFindSubCard(eCK eCard);
    Card_Table* pFindTableCard(char cTableSyn, Card*& rpMainCard);

    Card* pGetChild(i8 i_Child);
    Card* pGetChild(i8 i_Child, eCK& reCard);

    Card* pPopChild(void);

    void vRemoveChild(Card* pChildCard);
    void vRemoveSubCard(Card* pSubCard);
    void vReplaceChildWithItsSubs(Card* pChildCard);
    void vReplaceChild(Card* pChildCard, Card* pWithCard);

    void vTransferChildren(Card* pFromCard);
    void vTransferFrom(Card* pCards, Card* pFrom,
                       Card* pAt=nullptr, bool bAfter=true);
    void vTransferSubset(Card* pCards, Card* pFrom, Card* pTill,
                         Card* pAt=nullptr, bool bAfter=true);
    void vTransferTill(Card* pCards, Card* pTill,
                       Card* pAt=nullptr, bool bAfter=true);


  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

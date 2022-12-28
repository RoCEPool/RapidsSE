/*============================================================================*/
/* Card_Bound.hpp -- Abstract Bound                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Bound : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Bound(void);
    Card_Bound(eCK eCard);
   ~Card_Bound(void) noexcept(false);

    static Card_Bound*    pBound_CreateFrom_cmpPred(Card_CmpPred* p_cmpPred,
                                                 char cTblSyn);
    static Card_Bound*    pBound_CreateFrom_PRED(Card_PRED* p_PRED,
                                              char cTblSyn);
    static Card_Bound*    pBound_New(Stz* szRelnOp);
  };

  Card_Bound* _pCard_Bound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

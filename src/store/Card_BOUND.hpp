/*============================================================================*/
/* Card_BOUND.hpp -- Abstract Bound                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_BOUND : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_BOUND(void);
    Card_BOUND(eCK eCard);
   ~Card_BOUND(void) noexcept(false);

    static Card_BOUND*    pBound_CreateFrom_cmpPred(Card_CmpPred* p_cmpPred,
                                                    char cTblSyn);
    static Card_BOUND*    pBound_CreateFrom_PRED(Card_PRED* p_PRED,
                                                 char cTblSyn);
    static Card_BOUND*    pBound_New(Stz* szRelnOp);
  };

  Card_BOUND* _pCard_BOUND(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

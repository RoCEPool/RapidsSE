/*============================================================================*/
/* Card_CoPtnJoinList.hpp -- Co-Partition Join List                            */
/*============================================================================*/
#pragma once

#include "Card.hpp"

#include "Card_CoPtnJoin.hpp"
#include "Card_CoPtnPredList.hpp"

namespace STORE
{
  class Card_CoPtnJoinList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CoPtnJoinList(void);
   ~Card_CoPtnJoinList(void) noexcept(false);

    static Card_CoPtnJoinList* pExtract(Card_CoPtnPredList* p_coPtnPredList);

    Card_CoPtnJoin* pFind(Card_CoPtnPred* p_coPtnPred);
  };

  Card_CoPtnJoinList* _pCard_CoPtnJoinList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

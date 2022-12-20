/*============================================================================*/
/* Card_CoPtnPredList.hpp -- Co-Partition Predicate List                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CoPtnPredList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CoPtnPredList(void);
   ~Card_CoPtnPredList(void) noexcept(false);

    static Card_CoPtnPredList* pExtract(Card_JoinPredList* p_joinPredList,
                                        Card_TableList* p_tableList);

  };

  Card_CoPtnPredList* _pCard_CoPtnPredList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

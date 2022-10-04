/*============================================================================*/
/* Card_PtndJoin.hpp -- Partitioned Join                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_TableList;

  class Card_PtndJoin : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PtndJoin(void);
   ~Card_PtndJoin(void) noexcept(false);

    bool bMayFollowAfter(Card_SubSet* pPriorPtnJoins_sofar);
    void vJoinEst(Card_TableList* p_tableList);
  };

  Card_PtndJoin* _pCard_PtndJoin(void* aAddr);
  bool _bIsCard_PtndJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

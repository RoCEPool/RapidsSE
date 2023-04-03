/*============================================================================*/
/* Card_LoadJoined.hpp -- Load of the actual Join                             */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_LoadJoined : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_LoadJoined(void);
   ~Card_LoadJoined(void) noexcept(false);

    i8 nEstJoinVol(i8 nNodes,
                   i8 nBuffersIn,
                   Card_TableList* p_tableList,
                   char cTblSyn_r);

  };

  Card_LoadJoined* _pCard_LoadJoined(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

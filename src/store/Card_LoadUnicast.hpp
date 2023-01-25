/*============================================================================*/
/* Card_LoadUnicast.hpp -- Load of Unicast (in load of join)                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_LoadUnicast : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_LoadUnicast(void);
   ~Card_LoadUnicast(void) noexcept(false);

    void vCalculateLoad(eXK eCast, i8 nBuffersIn, Card_TableList* p_tableList,
                        char cTblSyn_l, char cTblSyn_r);
  };

  Card_LoadUnicast* _pCard_LoadUnicast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

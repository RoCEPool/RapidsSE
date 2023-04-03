/*============================================================================*/
/* Card_LoadNetwork.hpp -- Load of Network (in load of join)                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_LoadNetwork : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_LoadNetwork(void);
   ~Card_LoadNetwork(void) noexcept(false);

    i8 nEstNetworkVol(i8 nNodes,
                      eXK eNextCast,
                      i8 nBuff_arriving,
                      Card_TableList* p_tableList,
                      char cTblSyn_r);
  };

  Card_LoadNetwork* _pCard_LoadNetwork(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

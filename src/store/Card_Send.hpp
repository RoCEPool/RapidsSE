/*============================================================================*/
/* Card_Send.hpp -- Send                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"
#include "eXK.h"

namespace STORE
{
  class Card_Send : public Card_Unit
  {
  protected:
  
  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Send(eCK eKind, eXK eSendKind);
   ~Card_Send(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
  };

  Card_Send* _pCard_Send(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

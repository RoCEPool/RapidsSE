/*============================================================================*/
/* Card_By.hpp -- By Card                                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "Table.hpp"
#include "eAL.h"

namespace STORE
{
  class Card_By : public Card
  {
  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_By(eCK eKind);
   ~Card_By(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);

  };

  Card_By* _pCard_By(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

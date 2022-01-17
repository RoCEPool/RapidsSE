/*============================================================================*/
/* Card_DropTable.hpp -- Drop Table                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_DropTable : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_DropTable(eCK eKind);
   ~Card_DropTable(void) noexcept(false);
  };

  Card_DropTable* _pCard_DropTable(void* aAddr);
  bool _bIsCard_DropTable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
  protected:
    
  private:
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_DropTable(eCK eKind);
   ~Card_DropTable(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
  
    Footer* aFooter(void);
  };

  Card_DropTable* _pCard_DropTable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_ByList.hpp -- List of By Cards                                        */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_ByList : public Card
  {  
  protected:
    
  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ByList(eCK eKind);
   ~Card_ByList(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    
  };

  Card_ByList* _pCard_ByList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

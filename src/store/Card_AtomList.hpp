/*============================================================================*/
/* Card_AtomList.hpp -- List of Atom Cards                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE { class Card_Atom; }

namespace STORE
{
  class Card_AtomList : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AtomList(void);
   ~Card_AtomList(void) noexcept(false);
  };

  Card_AtomList* _pCard_AtomList(void* aAddr);
  bool _bIsCard_AtomList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

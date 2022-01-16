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
  protected:
    
  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_AtomList(eCK eKind);
   ~Card_AtomList(void) noexcept(false);

    
  private:
    static void vDeleteThis_(Card* pCard);
    
    Footer* aFooter(void);
  };

  Card_AtomList* _pCard_AtomList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_TermList.hpp -- List of Terms                                         */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE { class Card_Atom; }

namespace STORE
{
  class Card_TermList : public Card
  {  
  protected:
    
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TermList(eCK eKind);
   ~Card_TermList(void) noexcept(false);

  
  private:
    static void vDeleteThis_(Card* pCard);
    
    Footer* aFooter(void);
  };

  Card_TermList* _pCard_TermList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

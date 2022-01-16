/*============================================================================*/
/* Card_MainSubquery.hpp -- Main Subquery                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_MainSubquery : public Card
  {  
  protected:
 
  private:
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_MainSubquery(eCK eKind, Stz* szQueryNm);
   ~Card_MainSubquery(void) noexcept(false);
    
  private:
    static void vDeleteThis_(Card* pCard);
  
    Footer* aFooter(void);
  };

  Card_MainSubquery* _pCard_MainSubquery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

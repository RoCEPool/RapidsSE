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
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_MainSubquery(Stz* szQueryNm);
   ~Card_MainSubquery(void) noexcept(false);
  };

  Card_MainSubquery* _pCard_MainSubquery(void* aAddr);
  bool _bIsCard_MainSubquery(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

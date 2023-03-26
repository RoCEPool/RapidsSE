/*============================================================================*/
/* Card_Type.hpp -- Type                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Type : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Type(Stz* szType);
    Card_Type(Card_Type* pType);
   ~Card_Type(void) noexcept(false);

    Card_Type* pClone(void);

    bool bIsSame(Card_Type* p_type_arg);


    Stz* szTypeShort(void);
  };

  Card_Type* _pCard_Type(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

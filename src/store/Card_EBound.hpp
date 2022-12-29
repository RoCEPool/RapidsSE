/*============================================================================*/
/* Card_EBound.hpp -- Equi-Bound                                              */
/*============================================================================*/
#pragma once

#include "Card_Bound.hpp"

namespace STORE
{
  class Card_EBound : public Card_Bound
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_EBound(void);
   ~Card_EBound(void) noexcept(false);

    Stz* szGet_tblColOdn(void);
  };

  Card_EBound* _pCard_EBound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

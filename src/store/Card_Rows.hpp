/*============================================================================*/
/* Card_Rows.hpp -- Rows                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_Rows : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Rows(i8 iRows);
   ~Card_Rows(void) noexcept(false);
  };

  Card_Rows* _pCard_Rows(void* aAddr);
  bool _bIsCard_Rows(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

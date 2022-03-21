/*============================================================================*/
/* Card_Debug.hpp -- Debug                                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_MAIN.hpp"

namespace STORE
{
  class Card_Debug : public Card_MAIN
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Debug(void);
   ~Card_Debug(void) noexcept(false);
  };

  Card_Debug* _pCard_Debug(void* aAddr);
  bool _bIsCard_Debug(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

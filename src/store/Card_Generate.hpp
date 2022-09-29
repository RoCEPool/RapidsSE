/*============================================================================*/
/* Card_Generate.hpp -- Generate                                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Generate : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Generate(i8 iGenerate);
   ~Card_Generate(void) noexcept(false);
  };

  Card_Generate* _pCard_Generate(void* aAddr);
  bool _bIsCard_Generate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_StreamModel.hpp -- Stream Model                                       */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StreamModel : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamModel(void);
   ~Card_StreamModel(void) noexcept(false);
  };

  Card_StreamModel* _pCard_StreamModel(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

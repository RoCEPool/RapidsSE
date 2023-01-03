/*============================================================================*/
/* Card_GbyAgg.hpp -- Aggreg-GroupBy                                          */
/*============================================================================*/
#pragma once

#include "Card_Project.hpp"

namespace STORE
{
  class Card_GbyAgg : public Card_Project
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_GbyAgg(void);
   ~Card_GbyAgg(void) noexcept(false);

    void vApplyStream(Card_StreamModel* pStreamModel); // prototype

  };

  Card_GbyAgg* _pCard_GbyAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

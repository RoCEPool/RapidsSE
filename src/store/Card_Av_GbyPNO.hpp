/*============================================================================*/
/* Card_Av_GbyPNO.hpp -- Aggregate View Group By Partition                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_GbyPNO : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_GbyPNO(void);
    ~Card_Av_GbyPNO(void) noexcept(false);
  };

  Card_Av_GbyPNO* _pCard_Av_GbyPNO(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

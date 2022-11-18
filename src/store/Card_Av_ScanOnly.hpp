/*============================================================================*/
/* Card_Av_ScanOnly.hpp -- Aggregate View Scan Only                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_ScanOnly : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_ScanOnly(void);
    ~Card_Av_ScanOnly(void) noexcept(false);
  };

  Card_Av_ScanOnly* _pCard_Av_ScanOnly(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

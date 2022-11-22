/*============================================================================*/
/* Card_Av_Project.hpp -- Aggregate View Project                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_Project : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_Project(void);
    ~Card_Av_Project(void) noexcept(false);
  };

  Card_Av_Project* _pCard_Av_Project(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_Av_FilterProject.hpp -- Filter Project                                */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Av_FilterProject : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Av_FilterProject(void);
    ~Card_Av_FilterProject(void) noexcept(false);
  };

  Card_Av_FilterProject* _pCard_Av_FilterProject(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

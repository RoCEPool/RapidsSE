/*============================================================================*/
/* Elem.hpp -- Elem iterator                                                    */
/*============================================================================*/
#pragma once

#include "Board.hpp"


namespace STORE
{
  class Elem
  {
  private:
    i8 m_iLow;
    i8 m_iHigh;
    i8 m_iNext;
    i8 m_iCurr;

  public:
    Elem(i8 iLow, i8 iHigh);
    Elem(Card* pCard);

    ~Elem(void) noexcept(false);

    i8 iCurr(void);
    bool bNext(void);

  };
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

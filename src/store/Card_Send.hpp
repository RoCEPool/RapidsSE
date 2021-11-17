/*============================================================================*/
/* Card_Send.hpp -- Send                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "eXK.h"

namespace STORE
{
  class Card_Send : public Card
  {
  protected:
    eXK m_eSendKind;
  
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Send(eXK eSendKind);
   ~Card_Send(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_Send* _pCard_Send(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_Debug.hpp -- Debug                                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "eXK.h"

namespace STORE
{
  class Card_Debug : public Card
  {
  protected:
  
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Debug(void);
   ~Card_Debug(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Footer* aFooter(void);
  };

  Card_Debug* _pCard_Debug(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

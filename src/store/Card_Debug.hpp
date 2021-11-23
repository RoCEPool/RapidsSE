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
    s32 m_sOption;
  
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Debug(Stz* szOption);
   ~Card_Debug(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);
  
    Stz* szOption(void);
    void vOption(Stz* szOption);

    Footer* aFooter(void);
  };

  Card_Debug* _pCard_Debug(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

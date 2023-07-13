/*============================================================================*/
/* CardImage.hpp -- Card Image                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "eCK.h"
#include "eAL.h"

namespace STORE
{

  static const i8 C_CARDIMAGE_LINE  = 32;
  static const i8 C_CARDIMAGE_LINES = (C_SIZE_SMALL_OBJECT - 128) / C_CARDIMAGE_LINE;

  struct _CardLine {
    char m_zCardLine[C_CARDIMAGE_LINE];
  };

  class Card;

  class CardImage : public Object
  {
  private:
    Card*     m_pCard;
    eCK       m_eCard;
    eAL       m_eLevel;
    i8        m_iSLB;
    i8        m_iXform;

    i8        m_i_Line;
    _CardLine m_xaCardLines[C_CARDIMAGE_LINES];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardImage(Card* pCard, i8 iSLB, i8 iXform);
   ~CardImage(void) noexcept(false);

    void vAddLine(Stz* szLine);

    eCK  eCard(void);

    void vDump(i8 i_Tab=0);
    eAL  eLevel(void);

    Stz* szLine(i8 i_Line);
    i8   nLines(void);

    void vPrintLine(i8 i_Line);

  protected:
    Footer* aFooter(void);
  };

  CardImage* _pCardImage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

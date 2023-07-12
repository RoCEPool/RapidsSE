/*============================================================================*/
/* CardImage.hpp -- Card Image                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  static const i8 C_CARDIMAGE_LINE  = 32;
  static const i8 C_CARDIMAGE_LINES = (C_SIZE_SMALL_OBJECT - 128) / C_CARDIMAGE_LINE;

  struct _CardLine {
    char m_cCardLine[C_CARDIMAGE_LINE];
  };

  class CardImage : public Object
  {
  private:
    i8 m_i_Line;
    _CardLine m_xaCardLines[C_CARDIMAGE_LINES];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardImage(void);
   ~CardImage(void) noexcept(false);

    void vAddLine(char *azLine);

    char* azLine(i8 i_Line);
    i8    nLines(void);

  protected:
    Footer* aFooter(void);
  };

  CardImage* _pCardImage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

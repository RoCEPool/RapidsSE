/*============================================================================*/
/* Card_StreamDef.hpp -- Stream Definition Card                               */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "StreamDef.hpp"

namespace STORE
{
  class Card_StreamDef : public Card
  {  
  protected:
    StreamDef* m_pStreamDef;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamDef(StreamDef* pStreamDef);
   ~Card_StreamDef(void) noexcept(false);

    StreamDef* pStreamDef(void);
    void vStreamDef(StreamDef* pStreamDef);

  private:
    static void vDeleteThis_(Card* pCard);
    static void vBoardPrint_(Card* pCard, Ribbon* pRibbon, i8 iDepth);

    Footer* aFooter(void);
  };

  Card_StreamDef* _pCard_StreamDef(void* aAddr);
  bool _bIsCard_StreamDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

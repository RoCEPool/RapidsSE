/*============================================================================*/
/* Card_Stream.hpp -- Stream                                                  */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"
#include "Card_StmColList.hpp"

namespace STORE
{
  class Card_Stream : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Stream(void);
   ~Card_Stream(void) noexcept(false);

    void vStreamPrint(Ribbon* pRibbon, i8 iDepth);
   };

  Card_Stream* _pCard_Stream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

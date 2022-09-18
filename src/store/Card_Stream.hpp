/*============================================================================*/
/* Card_Stream.hpp -- Stream                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"
#include "Card_StreamColList.hpp"

namespace STORE
{
  class Card_Stream : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Stream(void);
   ~Card_Stream(void) noexcept(false);

    void vApplyOrder(Card_Stream* pCard_Stream_src);

    Card_StreamCol*     pGet_streamCol(i8 i_Col);
    Card_StreamColList* pGet_streamColList(void);

    i8 nStreamCols(void);
   };

  Card_Stream* _pCard_Stream(void* aAddr);
  bool _bIsCard_Stream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

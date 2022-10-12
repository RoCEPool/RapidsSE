/*============================================================================*/
/* Card_IdxColList.hpp -- Index Column List                                   */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Stream;

  class Card_IdxColList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IdxColList(Index* pIndex);
   ~Card_IdxColList(void) noexcept(false);

    Card_IdxColNm* pFind_idxCol(Stz *szColNm, i8& i_IdxColNo);

    void vOrdinate(Card_Stream* pCard_Stream);
  };

  Card_IdxColList* _pCard_IdxColList(void* aAddr);
  bool _bIsCard_IdxColList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

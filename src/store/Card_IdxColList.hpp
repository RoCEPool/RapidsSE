/*============================================================================*/
/* Card_IdxColList.hpp -- Index Column List                                   */
/*============================================================================*/
#pragma once

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_Stream;

  class Card_IdxColList : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IdxColList(Index* pIndex);
   ~Card_IdxColList(void) noexcept(false);

    Card_IdxCol* pFind_idxCol(Stz *szOrdinal, i8& i_IdxColNo);

    void vOrdinate(Card_Stream* pCard_Stream);
  };

  Card_IdxColList* _pCard_IdxColList(void* aAddr);
  bool _bIsCard_IdxColList(void* aAddr);
  Card_IdxColList* _pGetSub_Card_IdxColList(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

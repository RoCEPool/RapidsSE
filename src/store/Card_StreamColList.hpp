/*============================================================================*/
/* Card_StreamColList.hpp -- Stream Column List                                */
/*============================================================================*/
#pragma once

#include "Card_StreamCol.hpp"

namespace STORE
{
  class Card_AggElist;

  class Card_StreamColList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamColList(void);
    Card_StreamColList(StreamDef* pStreamDef, Stz* szPrefix_opt=nullptr);
    Card_StreamColList(Card_AggElist* pCard_AggElist);
   ~Card_StreamColList(void) noexcept(false);

    Card_StreamCol* pGet_streamCol(i8 i_StreamColNo);
    Card_StreamCol* pFind_streamCol(Stz *szColNm, i8& i_StreamColNo);

    void vPrependToAs(Stz* szPrefix);
    void vPopulate(StreamDef* pStreamDef, Stz* szPrefix_opt=nullptr);
    void vPopulate(Card_AggElist* pCard_AggElist);

    i8 nStreamCols(void);
  };

  Card_StreamColList* _pCard_StreamColList(void* aAddr);
  bool _bIsCard_StreamColList(void* aAddr);
  Card_StreamColList* _pSub_StreamColList(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_StmColList.hpp -- Stream Column List                                  */
/*============================================================================*/
#pragma once

#include "Card_StmCol.hpp"

namespace STORE
{
  class Card_AggElist;

  class Card_StmColList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StmColList(void);
    Card_StmColList(StreamDef* pStreamDef, Stz* szPrefix_opt=nullptr);
    Card_StmColList(Card_AggElist* pCard_AggElist);
   ~Card_StmColList(void) noexcept(false);

    Card_StmCol* pGet_streamCol(i8 i_StreamColNo);
    Card_StmCol* pFind_streamCol(Stz *szColNm, i8& i_StreamColNo);

    void vPrependToAs(Stz* szPrefix);
    void vPopulate(StreamDef* pStreamDef, Stz* szPrefix_opt=nullptr);
    void vPopulate(Card_AggElist* pCard_AggElist);

    i8 nStreamCols(void);
  };

  Card_StmColList* _pCard_StmColList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

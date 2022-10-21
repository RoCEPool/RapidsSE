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

    void vApplyOrder(Card_Stream* pCard_Stream_src);

    bool bContainsOrdinal(Stz* szOrd);
    bool bContainsPred(Card_PRED* pCard_PRED);

    Card_StmCol*     pFind_streamCol(Stz* szColNm);

    Card_StmCol*     pGet_streamCol(i8 i_Col);
    Card_StmCol*     pGet_streamCol(Stz* szStreamColOrd);
    Card_StmColList* pGet_streamColList(void);

    void vOrdinate(String* psOrdinal, Stz* szOrdinal);

    i8 nStreamCols(void);
    void vStreamPrint(Ribbon* pRibbon, i8 iDepth);

   };

  Card_Stream* _pCard_Stream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

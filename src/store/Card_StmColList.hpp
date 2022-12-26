/*============================================================================*/
/* Card_StmColList.hpp -- Stream Column List                                  */
/*============================================================================*/
#pragma once

#include "Card_StmCol.hpp"

namespace STORE
{
  class Card_StmColList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StmColList(void);
   ~Card_StmColList(void) noexcept(false);

    Card_StmCol* pStmColList_Find_streamCol(Stz *szColNm, i8& ri_StreamColNo);
    Card_StmCol* pStmCol_FindBy_tblColOdn(Stz *szTblColOdn, i8& ri_StmCol);
  };

  Card_StmColList* _pCard_StmColList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_TableList.hpp -- Table List                                           */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_TableList : public Card_symbol<Card_TableList>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TableList(void);
   ~Card_TableList(void) noexcept(false);

    static Card_TableList* pExtractList(Card_Deck* p_deck);

    Card_Table* pFind(char cTblSyn);

    Card_RowCount*  pGetRowCount(char cTblSyn);
    Card_RowSize*   pGetRowSize(char cTblSyn);
    Card_TablePtn*  pGetTablePtn(char cTblSyn);
    i8              nGetTableSize(char cTblSyn);
    Card_TableSize* pGetTableSize(char cTblSyn);

    Stz* szToTableNm(char cTblSyn);
  };

  Card_TableList* _pCard_TableList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

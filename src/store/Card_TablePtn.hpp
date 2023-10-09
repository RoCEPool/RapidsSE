/*============================================================================*/
/* Card_TablePtn.hpp -- Table Partition                                       */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_TablePtn : public Card_symbol<Card_TablePtn>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TablePtn(void);
   ~Card_TablePtn(void) noexcept(false);

   static Card_TablePtn* pDerive(Card_Table* p_table);

   static bool bColumnMatchesPtn(Symbol* p_Y_tblPtn, Symbol* p_Y_tblCol);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_TablePtn* _pCard_TablePtn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

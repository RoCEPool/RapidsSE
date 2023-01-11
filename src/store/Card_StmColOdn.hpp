/*============================================================================*/
/* Card_StmColOdn.hpp -- Stream Column Ordinal                                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StmColOdn : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StmColOdn(void);
   ~Card_StmColOdn(void) noexcept(false);

    static Card_StmColOdn* pResolve_tblColOdn_Into_stmColOdn(CardIter* pCI, Card_Term* p_term_tblColOdn, i8 i_Before);
  };

  Card_StmColOdn* _pCard_StmColOdn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

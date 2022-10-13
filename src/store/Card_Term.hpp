/*============================================================================*/
/* Card_Term.hpp -- Term                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "eTE.h"

namespace STORE
{
  class Card_Term : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Term(Stz* szTerm);
    Card_Term(Token* pTkn);
    Card_Term(Column* pColumn);
   ~Card_Term(void) noexcept(false);

    static void vTableColOrd(String* psTableColOrdinal, char cTableSyn, i8 iTableColNo);

    ePB  eBound(void);
    i8   iIndexColNo(void);
    bool bIsAggOp(void);
    bool bIsCommaSeparator(void);
    bool bIsIndexColOrdinal(void);
    bool bIsStreamColOrdinal(void);
    bool bIsTableColOrdinal(void);
    eRO  eRelOp(void);
    i8   iStreamColNo(void);
    i8   iTableColNo(void);
    char cTableSyn(void);
    eTE  eTermEval(void);
  };

  Card_Term* _pCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

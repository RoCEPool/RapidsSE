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

    Card_Term(void);
    Card_Term(Stz* szTerm);
   ~Card_Term(void) noexcept(false);

    void vBump_KeyOrdinal(i8 nBy=1);
    void vBump_StreamColOrdinal(i8 nBy=1);
    ePB  eEvalBound(void);
    i8   iGetOdnNo(void);
    eRO  eEvalRelOp(void);
    char cGetTableSyn(void);
    void vMarkIfMatchesAn_idxCol(Card_IdxColList* p_idxColList);
    eTE  eTermEval(void);
  };

  Card_Term* _pCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

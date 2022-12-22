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

    ePB  eEvalBound(void);
    i8   iGetOdnNo(void);
    eRO  eEvalRelOp(void);
    char cGetTableSyn(void);
    eTE  eTermEval(void);
  };

  Card_Term* _pCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

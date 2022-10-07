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

    bool bIsAggOp(void);
    bool bIsOrdinalIndexTerm(void);
    bool bIsOrdinalStreamTerm(void);
    bool bIsSeparator(void);

    i8   iOrdinalStreamColNo(void);
    i8   iOrdinalIndexColNo(void);
  };

  Card_Term* _pCard_Term(void* aAddr);
  void _Card_Term(void* aAddr);
  bool _bIsCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

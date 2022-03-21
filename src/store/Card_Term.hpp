/*============================================================================*/
/* Card_Term.hpp -- Term                                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_SUB.hpp"
#include "eTE.h"

namespace STORE
{
  class Card_Term : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Term(Stz* szTerm);
    Card_Term(Token* pTkn);
    Card_Term(Column* pColumn);
   ~Card_Term(void) noexcept(false);
  };

  Card_Term* _pCard_Term(void* aAddr);
  void _Card_Term(void* aAddr);
  bool _bIsCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

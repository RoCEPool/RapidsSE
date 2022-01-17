/*============================================================================*/
/* Card_Term.hpp -- Term Card                                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Term : public Card
  {
  protected:

  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Term(eCK eKind, Stz* szTerm);
   ~Card_Term(void) noexcept(false);

   private:
    static void vDeleteThis_(Card* pCard);
  };

  Card_Term* _pCard_Term(void* aAddr);
  void _Card_Term(void* aAddr);
  bool _bIsCard_Term(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

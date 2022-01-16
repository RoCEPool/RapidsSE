/*============================================================================*/
/* Card_Atom.hpp -- Atom Card                                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Atom : public Card
  {
  protected:

  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Atom(eCK eKind, Stz* szAtom);
   ~Card_Atom(void) noexcept(false);

   private:
    static void vDeleteThis_(Card* pCard);

    Footer* aFooter(void);
  };

  Card_Atom* _pCard_Atom(void* aAddr);
  void _Card_Atom(void* aAddr);
  bool _bIsCard_Atom(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Card_Property.hpp -- Atom Card                                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"

namespace STORE
{
  class Card_Property : public Card
  {
  protected:

  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Property(eCK eKind, Stz* szAtom);
   ~Card_Property(void) noexcept(false);

   private:
    static void vDeleteThis_(Card* pCard);

    Footer* aFooter(void);
  };

  Card_Property* _pCard_Property(void* aAddr);
  void _Card_Property(void* aAddr);
  bool _bIsCard_Property(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

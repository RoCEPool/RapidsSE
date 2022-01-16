/*============================================================================*/
/* Card_Scan.hpp -- Scan Card                                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_Unit.hpp"

namespace STORE
{
  class Card_Scan : public Card_Unit
  {
  protected:
    
  private:

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Scan(eCK eKind);
   ~Card_Scan(void) noexcept(false);

  private:
    static void vDeleteThis_(Card* pCard);

    Footer* aFooter(void);
  };

  Card_Scan* _pCard_Scan(void* aAddr);
  bool _bIsCard_Scan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

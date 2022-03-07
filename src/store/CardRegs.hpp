/*============================================================================*/
/* CardRegs.hpp -- Card Registers                                             */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "eCK.h"

namespace STORE
{
  class CardRegs : public Object
  {
  protected:
    Card* m_paCard[CK_ENUMS];
    b8    m_baFlag[CK_ENUMS];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardRegs(void);
   ~CardRegs(void) noexcept(false);
  
  protected:
    Footer* aFooter(void);
  };

  CardRegs* _pCardRegs(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

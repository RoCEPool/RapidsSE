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
    Card* m_paCardReg[CK_ENUMS];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardRegs(void);
   ~CardRegs(void) noexcept(false);
  
    Card* pCardReg(eCK eCard);
    void  vCardReg(Card* pCard);

    void  vClearCardReg(eCK eCard);
    void  vClearCardRegs(void);

    bool  bInCardReg(Card* pCard);
  
    Card* pSearchReg(eCK eCard);

  protected:
    Footer* aFooter(void);
  };

  CardRegs* _pCardRegs(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* CardUsage.hpp -- Card Usage                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_OP.hpp"

#include "eAL.h"
#include "eCK.h"

namespace STORE
{
  class CardUsage : public Object
  {
  private:

    eAL  m_eLevel;

    bool m_baSawCard[CK_ENUMS];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardUsage(void);
   ~CardUsage(void) noexcept(false);

    bool bAddCombined(Card_OP* p_OP);
    bool bAddNext(Card_OP* p_OP);
    bool bAddStage(Card_OP* p_OP);

    void vClear(eCK eCard);
    void vClearAll(void);

    bool bSaw(eCK eCard);
    i8   nSaw(void);
    void vSaw(Card_OP* p_OP);

  protected:
    Footer* aFooter(void);
  };

  CardUsage* _pCardUsage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

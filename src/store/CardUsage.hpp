/*============================================================================*/
/* CardUsage.hpp -- Card Usage                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eCK.h"

namespace STORE
{
  class CardUsage : public Object
  {
  private:

    bool m_baUsed[CK_ENUMS];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardUsage(void);
   ~CardUsage(void) noexcept(false);

    void vClear(eCK eCard);
    void vClearAll(void);

    bool bUsed(eCK eCard);
    void vUsed(eCK eCard, bool bUsed=true);

  protected:
    Footer* aFooter(void);
  };

  CardUsage* _pCardUsage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

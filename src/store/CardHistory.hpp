/*============================================================================*/
/* CardHistory.hpp -- Card History                                            */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_OP.hpp"

#include "eAL.h"
#include "eCK.h"

namespace STORE
{
  class CardHistory : public Object
  {
  private:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardHistory(void);
   ~CardHistory(void) noexcept(false);

  protected:
    Footer* aFooter(void);
  };

  CardHistory* _pCardHistory(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* CardImage.hpp -- Card Image                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_OP.hpp"

#include "eAL.h"
#include "eCK.h"

namespace STORE
{
  class CardImage : public Object
  {
  private:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardImage(void);
   ~CardImage(void) noexcept(false);

  protected:
    Footer* aFooter(void);
  };

  CardImage* _pCardImage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

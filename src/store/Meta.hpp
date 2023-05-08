/*============================================================================*/
/* Meta.hpp -- Abstract Metadata                                              */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eCK.h"

namespace STORE
{
  class Meta : public Object
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:

    Meta(eCN eClass);

   ~Meta(void) noexcept(false);


  };

  Meta* _pMeta(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

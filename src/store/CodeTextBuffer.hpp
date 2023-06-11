/*============================================================================*/
/* CodeTextBuffer.hpp -- Code Text Buffer                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class CodeTextBuffer : public Object
  {
  public:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    CodeTextBuffer(void);
   ~CodeTextBuffer(void) noexcept(false);

  protected:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* CodeText.hpp -- Code Text                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class CodeText : public Object
  {
  public:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    CodeText(void);
    ~CodeText(void) noexcept(false);

  protected:
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

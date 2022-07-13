/*============================================================================*/
/* LocalStr.hpp -- Local String                                               */
/*============================================================================*/
#pragma once

#include "eSI.h"

namespace BASE
{
  static const i8 C_LOCAL_STRAREA  = 512 - (2 * sizeof(i8));

//class Local;
  class TypeValueDscr;

  class LocalStr
  {
  public:
    TypeValueDscr* m_pLocal;
    i8             m__dummy0;
  
    char           m_xStrArea[C_LOCAL_STRAREA];

  public:
    LocalStr(void);
   ~LocalStr(void) noexcept(false);
  
    // auto conversion
    operator LocalStr*(void);

  private:
  
  };

  LocalStr* _pLocalStr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

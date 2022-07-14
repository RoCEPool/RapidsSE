/*============================================================================*/
/* LocalStr.hpp -- Local String                                               */
/*============================================================================*/
#pragma once

#include "eSI.h"

namespace BASE
{
  static const i8 C_LOCAL_AREA  = 512 - (2 * sizeof(i8));

//class Local;
  class TypeValueDscr;

  class LocalStr
  {
  public:
    TypeValueDscr* m_aLocal;
    i8             m__dummy0;
  
    char           m_caArea[C_LOCAL_AREA];

  public:
    LocalStr(void);
   ~LocalStr(void) noexcept(false);
  
    // auto conversion
    operator LocalStr*(void);

    char* azArea(void);

    TypeValueDscr* aLocal(void);
    void  vLocal(TypeValueDscr* aLocal);

  private:
  
  };

  LocalStr* _pLocalStr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* LocalStr.hpp -- Local String                                               */
/*============================================================================*/
#pragma once

#include "SizeArea.hpp"

namespace BASE
{
  class LocalStr
  {
  public:
    i8       m_i_LclStr;
    i8       m_i_NextFree;  // C_UNKNOWN indicates end

    Text*    m_pText;

  public:
    LocalStr(void);
   ~LocalStr(void) noexcept(false);
  
    // auto conversion
    operator LocalStr*(void);

    Text*    pText(void);
  };

  LocalStr* _pLocalStr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

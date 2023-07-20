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

    SizeArea m_xSizeArea;

  public:
    LocalStr(void);
   ~LocalStr(void) noexcept(false);
  
    // auto conversion
    operator LocalStr*(void);

    Text*    pText(void);

    char* azArea(void);
    Stz*  szArea(void);

    void  vReInitialise(void);
    SizeArea* axSizeArea(void);
  
    i8 jStrSize(void);  
  };

  LocalStr* _pLocalStr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

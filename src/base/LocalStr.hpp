/*============================================================================*/
/* LocalStr.hpp -- Local String                                               */
/*============================================================================*/
#pragma once

#include "SizeArea.hpp"

namespace BASE
{
  static const i8 C_LOCAL_AREA  = 512 - (2 * sizeof(i8));

//class Local;
  class TypeValueDscr;

  class LocalStr
  {
  public:
  
    LocalStrs* m_pLocalStrs;
    i8         m_i_LclStr;
    i8         m_i_NextFree;  // C_UNKNOWN indicates end
    
    SizeArea   m_xSizeArea;

  public:
    LocalStr(void);
   ~LocalStr(void) noexcept(false);
  
    // auto conversion
    operator LocalStr*(void);

    char* azArea(void);
    Stz*  szArea(void);

    SizeArea* axSizeArea(void);
  
    i8 jStrSize(void);

  private:
  
  };

  LocalStr* _pLocalStr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* LocalStr.hpp -- Local String                                               */
/*============================================================================*/
#pragma once

#include "SizeArea.hpp"

namespace BASE
{
  class LocalStr
  {
    friend class LocalStrs;
  private:
    i8    m_i_LclStr;
    i8    m_i_NextFree;  // C_UNKNOWN indicates end

    Text* m_pText;

  public:
    LocalStr(void);
   ~LocalStr(void) noexcept(false);

    void  vEnsureText(void);

    void  vInitialise(i8 i_LclStr, i8 i_NextFree);

    i8   i_LocalStr(void);

    i8   i_NextFree(void);
    void v_NextFree(i8 i_NextFree);

    Stz* szText(void);
    void vText(Stz* szText);
  };

  LocalStr* _pLocalStr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

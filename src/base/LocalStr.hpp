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

    void  vInitialise(i8 i_LclStr, i8 i_NextFree);

    void  vEnsureText(void);

    //Text* pText(void);

    Stz* szText(void);
    void vText(Stz* szText);
  };

  LocalStr* _pLocalStr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

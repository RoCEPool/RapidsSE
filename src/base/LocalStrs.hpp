/*============================================================================*/
/* LocalStrs.hpp -- Local Strings                                             */
/*============================================================================*/
#pragma once

#include "LocalStr.hpp"

namespace BASE
{
  static const i8 C_LOCAL_STRS = (C_SIZE_MEDIUM_OBJECT - 128) / sizeof(LocalStr);

  class LocalVars;

  class LocalStrs : public Object
  {
  public:
    LocalVars* m_pLocalVars;
    i8         m_i_FirstFree;  // C_UNKNOWN indicates end
    LocalStr   m_xaLocalStr[C_LOCAL_STRS];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LocalStrs(LocalVars* pLocalVars);
   ~LocalStrs(void) noexcept(false);
      
    LocalStr* axAllotLocalStr(void);
    void     vFreeLocalStr(LocalStr* axLocalStr);

    // must be last
    Footer* aFooter(void);
  };
  
  LocalStrs* _pLocalStrs(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

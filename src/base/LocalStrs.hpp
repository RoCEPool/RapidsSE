/*============================================================================*/
/* LocalStrs.hpp -- Local Strings                                             */
/*============================================================================*/
#pragma once

#include "LocalStr.hpp"

namespace BASE
{
  static const i8 C_LOCAL_STRS = (C_SIZE_MEDIUM_OBJECT - 128) / sizeof(LocalStr);
  static const i8 C_LOCAL_STRS_reduced = 16;

  class LocalVars;

  class LocalStrs : public Object
  {
  public:
    i8         m_i_FirstFree;  // C_UNKNOWN indicates end
    LocalStr   m_xaLocalStr[C_LOCAL_STRS_reduced];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LocalStrs(void);
   ~LocalStrs(void) noexcept(false);
      
    LocalStr* axAllot(void);
    void     vRevert(LocalStr* axLocalStr);

    // must be last
    Footer* aFooter(void);
  };
  
  LocalStrs* _pLocalStrs(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

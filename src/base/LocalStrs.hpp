/*============================================================================*/
/* LocalStrs.hpp -- Local Strings                                             */
/*============================================================================*/
#pragma once

#include "LocalStr.hpp"

namespace BASE
{

  class LocalVars;

  class LocalStrs : public Object
  {
    static const i8 C_LOCAL_STRS = 16;

  public:
    i8         m_i_FirstFree;  // C_UNKNOWN indicates end
    LocalStr   m_xaLocalStr[C_LOCAL_STRS];

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LocalStrs(void);
   ~LocalStrs(void) noexcept(false);

    static void vRepair(Object* pObject);

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

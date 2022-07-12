/*============================================================================*/
/* LocalVars.hpp -- Local Variables                                           */
/*============================================================================*/
#pragma once

#include "TypeValue.hpp"

namespace BASE
{
  struct _LclVar {
    i8      m__dummy0;
    i8      m__dummy1;
    TypeValue m_xSenCell;
  };

  static const i8 C_LOCAL_VARS = (C_SIZE_MEDIUM_OBJECT - 128) / sizeof(_LclVar);

  class LocalStrs;

  class LocalVars : public Object
  {
  public:
  
    LocalStrs* m_pLocalStrs;
  
    _LclVar m_xaLclVar[C_LOCAL_VARS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LocalVars(void);
   ~LocalVars(void) noexcept(false);
      
    // must be last
    Footer* aFooter(void);
  };
  
  LocalVars* _pLocalVars(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

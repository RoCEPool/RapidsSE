/*============================================================================*/
/* LocalVars.hpp -- Local Variables                                           */
/*============================================================================*/
#pragma once

#include "TypeValue.hpp"
#include "LocalVar.hpp"

namespace BASE
{
  static const i8 C_LOCAL_VARS = (C_SIZE_MEDIUM_OBJECT - 128) / sizeof(LocalVar);

  class LocalStrs;

  class LocalVars : public Object
  {
  public:
  
    LocalStrs* m_pLocalStrs;
  
    i8 m_nLocalVars;
  
    LocalVar m_xaLocalVar[C_LOCAL_VARS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LocalVars(void);
   ~LocalVars(void) noexcept(false);
  
    void vFramePop(void);
    void vFramePush(i8 nLocals, void* aDiscardPoint);

    LocalVar* axLocalVar(i8 i_Local);

    LocalStr* axLocalStrAllot(void);
    void vLocalStrRevert(LocalStr* axLocalStr);
  
    // must be last
    Footer* aFooter(void);
  };
  
  LocalVars* _pLocalVars(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

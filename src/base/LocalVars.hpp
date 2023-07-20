/*============================================================================*/
/* LocalVars.hpp -- Local Variables                                           */
/*============================================================================*/
#pragma once

#include "TypeValue.hpp"
#include "LocalVar.hpp"

namespace BASE
{
  static const i8 C_LOCAL_VARS = (C_SIZE_SMALL_OBJECT - 128) / sizeof(LocalVar);

  class LocalStrs;

  class LocalVars : public Object
  {
  public:
  
    LocalStrs* m_p_LocalStrs;
  
    i8 m_nLocalVars;
  
    LocalVar m_xaLocalVar[C_LOCAL_VARS];
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    LocalVars(LocalStrs* pLocalStrs);
   ~LocalVars(void) noexcept(false);
  
    void* aDiscardPoint(void);

    void vFramePop(void* aDiscardPoint);
    bool bFramePush(i8 nLocals, void* aDiscardPoint);

    LocalVar* axLocalVar(i8 i_Local);

    LocalStr* axLocalStrAllot(void);
    void vLocalStrRevert(LocalStr* axLocalStr);
 
    //void vVerify(void);

    // must be last
    Footer* aFooter(void);
  };
  
  LocalVars* _pLocalVars(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* LocalVar.hpp -- Local Var                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace BASE
{
  class LocalStr;

  class LocalVar
  {
  public:
    LocalVars* m_pLocalVars;
    i8         m_i_LocalVar;
    void*      m_aDiscardPoint;
    TypeValue  m_xTypeValue;
    LocalStr*  m_axLocalStr;

  public:
    LocalVar(void);
   ~LocalVar(void) noexcept(false);
  
    // auto conversion
    operator LocalVar*(void);
  

    void* aDiscardPoint(void);
    i8    i_LocalVar(void);
    LocalStr* axLocalStr(void);
    LocalStr* axLocalStrAllot(void);
    void  vLocalStrRevert(void);
    i8    jStackUsage(void);
    i8    jStzSize(void);
    eCN   eType(void);
    _Header* axType(void);
    TypeValue* axTypeValue(void);
    Dtm*  axValue(void);

    void vSet(TypeValueArea* aTypeValueArea);
    void vSetType(TypeValueArea* aTypeValueArea);
    void vSetText(TypeValueArea* aTypeValueArea);
    void vSetValue(TypeValueArea* aTypedValueArea);

    void vGet(TypeValueArea* aTypeValueArea);
    void vGetType(TypeValueArea* aTypeValueArea);
    void vGetText(TypeValueArea* aTypeValueArea);
    void vGetValue(TypeValueArea* aTypedValueArea);

  private:
    void vEnsureText(void);
    void vNixText(void);
    void vSetSize(i8 jSize);
    void vSetValue(i8 iValue);
  };

  LocalVar* _pLocalVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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
    void*      m_aDiscardPoint;

    TypeValue  m_xTypeValue;
    LocalStr*  m_axLocalStr;

  public:
    LocalVar(void);
   ~LocalVar(void) noexcept(false);

    void* aDiscardPoint(void);
    void  vLocalStrAllot(void);
    void  vLocalStrRevert(void);
    i8    jStackUsage(void);
    i8    jStz(void);
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
    void vSetSize(i8 jStz);
    void vSetValue(i8 iValue);
  };

  LocalVar* _pLocalVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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

    Text*      m_pText;

  public:
    LocalVar(void);
   ~LocalVar(void) noexcept(false);
  
    // auto conversion
    operator LocalVar*(void);
  
    char* azArea(void);
    Stz*  szArea(void);

    void* aDiscardPoint(void);
    i8    i_LocalVar(void);
    LocalStr* axLocalStr(void);
    LocalStr* axLocalStrAllot(void);
    void  vLocalStrRevert(void);
    i8    nStackUsage(void);
    i8    jStzSize(void);
    eCN   eType(void);
    _Header* axType(void);
    TypeValue* axTypeValue(void);
    Dtm*  axValue(void);

    void vSet(TypeValueArea* aTypeValueArea);
    void vSetType(TypeValueArea* aTypeValueArea);
    void vSetText(TypeValueArea* aTypeValueArea);
    void vSetValue(TypeValueArea* aTypedValue);
  };

  LocalVar* _pLocalVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

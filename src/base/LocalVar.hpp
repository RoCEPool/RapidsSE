/*============================================================================*/
/* LocalVar.hpp -- Local Var                                                  */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace BASE
{
  class LocalVar
  {
  public:
    LocalVars* m_pLocalVars;
    i8         m_i_LocalVar;
    void*      m_aDiscardPoint;
    TypeValue  m_xTypeValue;
    LocalStr*  m_aLocalStr;

  public:
    LocalVar(void);
   ~LocalVar(void) noexcept(false);
  
    // auto conversion
    operator LocalVar*(void);


  };

  LocalVar* _pLocalVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

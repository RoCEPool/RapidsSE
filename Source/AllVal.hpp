/*============================================================================*/
/* AllVal.hpp - Union of all AnyVar sub-classes                               */
/*============================================================================*/
#pragma once

namespace SenVM
{
  union AllVal {
    
    AnyVal  m_xAnyVal;
    
    FipVal  m_xFipVal;
    IntVal  m_xIntVal;
    PtrVal  m_xPtrVal;
    StrVal  m_xStrVal;
    
    byte    m_yaSpace[C_48KB];
    
    void vInit(void) { MemLib::vZero(m_yaSpace,sizeof(m_yaSpace),CN_NONE); }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

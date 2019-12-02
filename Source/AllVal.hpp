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
    StzVal  m_xStzVal;
    
    byte    m_yaSpace[C_48KB];
    
    void vInit(void) { MemLib::vZero(m_yaSpace,sizeof(m_yaSpace),CN_NONE); }
  };

  union AllVal_small {
    
    AnyVal  m_xAnyVal;
    
    FipVal  m_xFipVal;
    IntVal  m_xIntVal;
    PtrVal  m_xPtrVal;
    StzVal  m_xStzVal;
    
    byte    m_yaSpace[C_8KB];
    
    void vInit(void) { MemLib::vZero(m_yaSpace,sizeof(m_yaSpace),CN_NONE); }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

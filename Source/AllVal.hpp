/*============================================================================*/
/* AllVal.hpp - Union of all AnyVar sub-classes                               */
/*============================================================================*/
#pragma once

namespace SenVM
{
  union AllVal {
    
    AnyVal  m_xAnyVal;
    
    FipVal  m_xFipVal;
    PtrVal  m_xPtrVal;
    
    BcdVal  m_xBcdVal;
    BolVal  m_xBolVal;
    DteVal  m_xDteVal;
    EdtVal  m_xEdtVal;
    FltVal  m_xFltVal;
    IntVal  m_xIntVal;
    IvlVal  m_xIvlVal;
    StzVal  m_xStzVal;
    TmeVal  m_xTmeVal;
    TspVal  m_xTspVal;

    byte    m_yaSpace[C_48KB];
    
    void vInit(void) { MemLib::vZero(m_yaSpace,sizeof(m_yaSpace),CN_NONE); }
  };

  // variations
  
  union AllVal_y32 {
    
    AnyVal  m_xAnyVal;
    
    FipVal  m_xFipVal;
    PtrVal  m_xPtrVal;
    
    BcdVal  m_xBcdVal;
    BolVal  m_xBolVal;
    DteVal  m_xDteVal;
    EdtVal  m_xEdtVal;
    FltVal  m_xFltVal;
    IntVal  m_xIntVal;
    IvlVal  m_xIvlVal;
    StzVal  m_xStzVal;
    TmeVal  m_xTmeVal;
    TspVal  m_xTspVal;

    byte    m_yaSpace[32];
    
    void vInit(void) { MemLib::vZero(m_yaSpace,sizeof(AllVal_y32),CN_NONE); }
  };

  union AllVal_y8KB {
    
    AnyVal  m_xAnyVal;
    
    FipVal  m_xFipVal;
    PtrVal  m_xPtrVal;
    
    BcdVal  m_xBcdVal;
    BolVal  m_xBolVal;
    DteVal  m_xDteVal;
    EdtVal  m_xEdtVal;
    FltVal  m_xFltVal;
    IntVal  m_xIntVal;
    IvlVal  m_xIvlVal;
    StzVal  m_xStzVal;
    TmeVal  m_xTmeVal;
    TspVal  m_xTspVal;

    byte    m_yaSpace[C_8KB];
    
    void vInit(void) { MemLib::vZero(m_yaSpace,sizeof(AllVal_y8KB),CN_NONE); }
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* AllVal.hpp - Union of all AnyVar sub-classes                               */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class AllVal
  {
  public:
   union {
      i8      m__zero__;
     
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
    };
  };

  // concreate variations
  
  class AllVal_y32 : public AnyVal
  {
    byte m_yaSpace[32];
    
  public:
    void vInit(void) {
      MemLib::vZero(&m__zero__,sizeof(AllVal_y32) - 8,CN_NONE);
      vMagic();
      vInt(C_DATUM_NULL);
    }
  };

  
  class AllVal_y8KB : public AnyVal
  {
    byte m_yaSpace[C_8KB];
    
  public:
    void vInit(void) {
      MemLib::vZero(&m__zero__,sizeof(AllVal_y8KB) - 8,CN_NONE);
      vMagic();
      vInt(C_DATUM_NULL);
    }
  };
  
  class AllVal_y48KB : public AnyVal
  {
  byte m_yaSpace[C_48KB];
  
  public:
    void vInit(void) {
      MemLib::vZero(&m__zero__,sizeof(AllVal_y48KB) - 8,CN_NONE);
      vMagic();
      vInt(C_DATUM_NULL);
    }
  };
  
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

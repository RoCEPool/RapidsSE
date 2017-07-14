/*============================================================================*/
/* Object.hpp -- Base Class for Objects in Buffer Pool                        */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  class Object
  {
  private:
    union {
      u8   m_uMagic;
      char m_sMagic[8];
    };
    eCN  m_eClass;
    bool m_bWordBody;
    char m_sFiller[3];
  
  public:
    Object(eCN eClass);
    void vInitMagic(void);
  
    void  vSetMagic(void);
    i8    iMagic(void) const;
  
    Str*  aClass(void);
    eCN eClass(void);
    void  vClass(eCN eClassNo);

    bool bWordBody(void);
    void vWordBody(bool bIsWordBody);
    
    Footer* aFooter(void);
  };
  
  /*--------------------------------------------------------------------------*/
  /* get class                                                                */
  /*--------------------------------------------------------------------------*/
  inline eCN Object::eClass(void)
  {
    return m_eClass;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

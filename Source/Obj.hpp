/*============================================================================*/
/* Obj.hpp -- Base Class for Proper Objects (short form)                      */
/*============================================================================*/
#pragma once

namespace MagicAlgebra
{
  class Obj
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
    Obj(eCN eClass);
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
  inline eCN Obj::eClass(void)
  {
    return m_eClass;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

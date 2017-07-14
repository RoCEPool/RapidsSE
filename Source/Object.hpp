/*============================================================================*/
/* Object.hpp -- Base Class for Objects in Buffer Pool                        */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  class Object :public Magic,
                public Class
  {
  private:
    bool m_bWordBody;
    char m_sFiller[3];
  
  public:
    Object(eCN eClass);
  
  
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

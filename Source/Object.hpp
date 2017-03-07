/*============================================================================*/
/* Object.hpp -- Base Class for Objects in Buffer Pool                        */
/*============================================================================*/
#pragma once

namespace Magic
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
  
    char* aClass(void);
    eCN   eClass(void) const;
    void  vClass(eCN eClassNo);

    bool bWordBody(void);
    void vWordBody(bool bIsWordBody);
    
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

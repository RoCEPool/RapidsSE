/*============================================================================*/
/* Header.hpp -- Header for instances needing magic & class                   */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  static const u8 C_MAGIC   = 0x4B696A4D; // MjiK
                              //K i j M
  
  class Header
  {
  protected:
    union {
      u4   m_uMagic;
      char m_sMagic[4];
    };
  private:
    eCN  m_eClass;
    bool m_bWordBody;
  
  public:
    Header(eCN eClass);
  
    bool  bValidMagic(void);
  
    Stz* aClass(void);
  
    bool bWordBody(void);
    void vWordBody(bool bIsWordBody);
    
    Footer* aFooter(void);
  
  /*--------------------------------------------------------------------------*/
  /* get class                                                                */
  /*--------------------------------------------------------------------------*/
  inline eCN eClass(void)
  {
    return m_eClass;
  }
  
  /*----------------------------------------------------------------------------*/
  /* set class                                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vSetClass(eCN eClass)
  {
    m_eClass = eClass;
  }
  
  /*----------------------------------------------------------------------------*/
  /* set magic                                                                  */
  /*----------------------------------------------------------------------------*/
  inline void vSetMagic(void)
  {
    m_uMagic = C_MAGIC;
  }
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

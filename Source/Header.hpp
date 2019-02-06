/*============================================================================*/
/* Header.hpp -- Header for instances needing magic & class                   */
/*============================================================================*/
#pragma once

namespace Correctness
{
  static const u4 C_MAGIC   = 0x4B696A4D; // MjiK
                              //K i j M
  
  class Header
  {
  protected:
    union {
      u4   m_uMagic;
      char m_caMagic[4];
    };
    eCN  m_eClass;
    bool m_bWordBody;
  
  public:
    Header(eCN eClass);
   ~Header(void) noexcept(false);

    void vAssertHeader(void);
    void vSprint(Stz* szHeader);
    bool bValidMagic(void);
    
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

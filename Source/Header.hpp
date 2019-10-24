/*============================================================================*/
/* Header.hpp -- Header for instances needing magic & class                   */
/*============================================================================*/
#pragma once

namespace Correctness
{
  static const u4 C_MAGIC   = 0x4B696A4D; // speels MjiK
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
    Header(void);
    Header(eCN eClass);
   ~Header(void) noexcept(false);

    void vAssertHeader(void);
    void vSprint(Stz* szHeader);
    
  /*--------------------------------------------------------------------------*/
  /* get class                                                                */
  /*--------------------------------------------------------------------------*/
  inline eCN eClass(void)
  {
    return m_eClass;
  }
  
  /*--------------------------------------------------------------------------*/
  /* test for given class                                                     */
  /*--------------------------------------------------------------------------*/
  inline bool bIsClass(eCN eClass)
  {
    if (m_eClass == eClass)
      return true;
    else
      return false;
  }

  /*--------------------------------------------------------------------------*/
  /* set class                                                                */
  /*--------------------------------------------------------------------------*/
  inline void vSetClass(eCN eClass)
  {
    m_eClass = eClass;
  }
  
  /*--------------------------------------------------------------------------*/
  /* set magic                                                                */
  /*--------------------------------------------------------------------------*/
  inline void vSetMagic(void)
  {
    m_uMagic = C_MAGIC;
  }
  
  /*--------------------------------------------------------------------------*/
  /* test for valid magic                                                     */
  /*--------------------------------------------------------------------------*/
  inline bool bValidMagic(void)
  {
    if (C_MAGIC == m_uMagic)
      return true;
    else
      return false;
  }
  

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

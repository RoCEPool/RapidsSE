/*============================================================================*/
/* Magic.hpp -- Magic header                                                  */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  class Magic
  {
  protected:
  union {
    u8   m_uMagic;
    char m_sMagic[8];
  };
  
  public:
    Magic(void);
    ~Magic(void);
  
  
    void vInitMagic(void);
    void  vSetMagic(void);
    i8    iMagic(void) const;
  
    void vAssertFoot(void) const;
    void vSetFoot(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

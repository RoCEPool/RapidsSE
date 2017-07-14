/*============================================================================*/
/* Magic.hpp -- Magic header                                                  */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  static const u8 C_MAGIC = 0x4F4E434947414D5E; // ^MAGICNO
  
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
  
    void  vMagic(void);
    i8    iMagic(void) const;
  };
}

/*--------------------------------------------------------------------------*/
/* check the object has a magic marker as its first word                    */
/*--------------------------------------------------------------------------*/
#ifdef __MAGIC_CHECK
  #define _magic_() ASSERT(iMagic() == C_MAGIC);
#else
  #define _magic_()
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

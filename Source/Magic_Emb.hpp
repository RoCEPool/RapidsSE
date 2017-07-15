/*============================================================================*/
/* Magic.hpp -- Magic header                                                  */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  static const u8 C_MAGIC_EMB = 0x4F4E434947414D5E; // ^MAGICNO
  
 class Magic_Emb
  {
  protected:
  u1   m_uMagic;
  
  public:
    Magic_Emb(void);
  
    void  vSetMagic(void);
    i8    iMagic(void) const;
  };
}

/*--------------------------------------------------------------------------*/
/* check the object has a magic marker as its first word                    */
/*--------------------------------------------------------------------------*/
#ifdef __MAGIC_CHECK
  #define _magic_emb_() ASSERT(iMagic() == C_MAGIC_EMB);
#else
  #define _magic_emb_()
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

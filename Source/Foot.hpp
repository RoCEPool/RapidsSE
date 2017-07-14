/*============================================================================*/
/* Foot.hpp -- Footer of instances (short form)                               */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  static const u8 C_FOOT = 0x245245544F4F4624; // $C_FOOTER$

  class Foot
  {
  private:
  union {
    u8   m_uFoot;
    char m_cFoot[8];
  };
  
  public:
    Foot(void);
    ~Foot(void);
    
    void vAssertFoot(void) const;
    void vSetFoot(void);
  };
}

/*----------------------------------------------------------------------------*/
/* macro for standard aFooter() function                                      */
/*----------------------------------------------------------------------------*/
#define _aFoot_(CLASSNM) \
  Foot* CLASSNM::aFoot(void) \
  { \
    _precondition_(_MAGIC_); \
    return &m_xFoot; \
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

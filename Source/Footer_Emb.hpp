/*============================================================================*/
/* Footer_Emb.hpp -- Footer of instances (short form)                               */
/*============================================================================*/
#pragma once

namespace CorrectnessAlgebra
{
  static const u1 C_FOOTER_EMB = 0x73;

  class Footer_Emb
  {
  private:
    u1   m_uFooter;
  
  public:
    Footer_Emb(void);
    ~Footer_Emb(void);
    
    void vAssertFooter(void) const;
    void vSetFooter(void);
  };
}

/*----------------------------------------------------------------------------*/
/* macro for standard aFooter() function                                      */
/*----------------------------------------------------------------------------*/
#define _aFooter_Emb_(CLASSNM) \
  Footer_Emb* CLASSNM::aFooter_Emb(void) \
  { \
    _precondition_(_MAGIC_); \
    return &m_xFooter; \
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

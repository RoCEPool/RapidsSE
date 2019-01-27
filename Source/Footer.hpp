/*============================================================================*/
/* footer.hpp -- footer for concrete classes                                  */
/*============================================================================*/
#pragma once

namespace Correctness
{
  static const u8 C_FOOTER = 0x245245544F4F4624; // $C_FOOTER$

  class Footer
  {
  private:
  union {
    u8   m_uFooter;
    char m_caFooter[8];
  };
  
  public:
    Footer(void);
    ~Footer(void) noexcept(false);
    
    void vAssertFooter(void) const;
    bool bValidFooter(void);
    void vSetFooter(void);
  };
}

/*----------------------------------------------------------------------------*/
/* macro for standard aFooter() function                                      */
/*----------------------------------------------------------------------------*/
#define __aFooterFunction(CLASSNM) \
  Footer* CLASSNM::aFooter(void) \
  { \
    _precondition_(_MAGIC_); \
    return &m_xFooter; \
  }

/*--------------------------------------------------------------------------*/
/* check the object has a footer marker as its last word                    */
/*--------------------------------------------------------------------------*/
#ifdef __FOOTER_CHECK
  #define _footer_() aFooter()->vAssertFooter();
  #define _hazard_() aHazard()->vAssertFooter();
  #define _setFooter_() aFooter()->vSetFooter();
#else
  #define _footer_()
  #define _hazard_()
  #define _setFooter_()
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

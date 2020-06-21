/*============================================================================*/
/* footer.hpp -- footer for concrete classes                                  */
/*============================================================================*/
#pragma once

namespace Correctness
{
  static const u8 C_FOOTER = 0x245245544F4F4624; // spells $FOOTER$
                             //$ R E T O O F $

  class Footer
  {
  private:
    i8 m__PadToDoubleWord;
  
    union {
      u8   m_uFooter;
      char m_caFooter[8];
    };
  
  public:
    Footer(void);
   ~Footer(void) noexcept(false);
    
    void vAssertFooter(void) const;
    void vSetFooter(void);
    void vSprint(Stz* szHeader);
    bool bValidFooter(void);
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
  } \
EndDefine

/*----------------------------------------------------------------------------*/
/* macro for standard aFooter() function                                      */
/*----------------------------------------------------------------------------*/
#define __aHazardFunction(CLASSNM,CN) \
  Footer* CLASSNM::aHazard(void) \
  { \
    _precondition_(_ADDRESS_,_MAGIC_,CN); \
    return &m_xFooter; \
  } \
EndDefine

/*--------------------------------------------------------------------------*/
/* check the object has a footer marker as its last word                    */
/*--------------------------------------------------------------------------*/
#ifdef WITH_CORECTNESS_ALGEBRA
  #define _footer_()    aFooter()->vAssertFooter(); EndDefine
  #define _hazard_()    aHazard()->vAssertFooter(); EndDefine
  #define _header_()    aHeader()->vAssertHeader(); EndDefine
  #define _setFooter_() aFooter()->vSetFooter(); EndDefine
#else
  #define _footer_() EndDefine
  #define _hazard_() EndDefine
  #define _header_() EndDefine
  #define _setFooter_() EndDefine
#endif

#ifdef WITH_CHECK_INITIALISED
  #undef _setFooter_
  #define _setFooter_() aFooter()->vSetFooter(); EndDefine
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

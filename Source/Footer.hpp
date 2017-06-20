/*============================================================================*/
/* footer.hpp -- footer for concrete classes                                  */
/*============================================================================*/
#pragma once

namespace Magic
{
  static const u8 FOOTER = 0x245245544F4F4624; // $FOOTER$

  class Footer
  {
  private:
  union {
    u8   m_uFooter;
    char m_cFooter[8];
  };
  
  public:
    Footer(void);
    ~Footer(void);
    
    void vAssertFooter(void) const;
    void vSetFooter(void);
  };
}

//template <class C>
//Footer* <C>::aFooter(void)
//{
//  _precondition_(_MAGIC_);
//  
//  return &m_xFooter;
//}

/*----------------------------------------------------------------------------*/
/* macro for standard aFooter() function                                      */
/*----------------------------------------------------------------------------*/
#define _pFooter_(CLASSNM) \
  Footer* CLASSNM::aFooter(void) \
  { \
    _precondition_(_MAGIC_); \
    return &m_xFooter; \
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* footer.hpp -- footer for concrete classes                                  */
/*============================================================================*/
#pragma once

#include "Magic.h"

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
    u8   uFooter(void);
    void vSetFooter(void);
  };
}

//template <class C>
//Footer* <C>::pFooter(void)
//{
//  _precondition_(_MAGIC_);
//  
//  return &m_xFooter;
//}

/*----------------------------------------------------------------------------*/
/* macro for standard pFooter() function                                      */
/*----------------------------------------------------------------------------*/
#define _pFooter_(CLASSNM) \
  Footer* CLASSNM::pFooter(void) \
  { \
    _precondition_(_MAGIC_); \
    return &m_xFooter; \
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* CardCtl.hpp -- Card Control                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eCK.h"

namespace STORE
{
  class CardCtl : public Object
  {
  protected:
    i8 m_iaPinForm[CK_ENUMS];
    bool m_baPrintSubCards[CK_ENUMS];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardCtl(void);
   ~CardCtl(void) noexcept(false);

    i8 iPinForm(i8 i_Card);
    void vPinForm(i8 i_Card, i8 iPrint);

    bool bPrintSubCards(i8 i_Card);
    void vPrintSubCards(i8 i_Card, bool bPrint);

  protected:
    Footer* aFooter(void);
  };

  CardCtl* _pCardCtl(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

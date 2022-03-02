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
    i8 m_iDepth;
    i8 m_iaForm[CK_ENUMS];
    i8 m_iaPrint[CK_ENUMS];
    i8 m_iaSee[CK_ENUMS];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CardCtl(void);
   ~CardCtl(void) noexcept(false);

    i8 iDepth(void);
    void vDepth(i8 iDepth);

    i8 iForm(i8 i_Card);
    void vForm(i8 i_Card, i8 iForm);

    i8 iPrint(i8 i_Card);
    void vPrint(i8 i_Card, i8 iPrint);

    i8 iSee(i8 i_Card);
    void vSee(i8 i_Card, i8 iForm);
  
  protected:
    Footer* aFooter(void);
  };

  CardCtl* _pCardCtl(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

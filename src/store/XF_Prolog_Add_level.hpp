/*============================================================================*/
/* XF_Prolog_Add_level.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Prolog_Add_level : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Add_level(CardIter* pCI);
   ~XF_Prolog_Add_level(void) noexcept(false);

    static void vXF_Add_level(CardIter* pCI);

    void vXF_Add_level(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Add_level* _pXF_Prolog_Add_level(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

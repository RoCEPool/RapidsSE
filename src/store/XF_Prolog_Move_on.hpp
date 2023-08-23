/*============================================================================*/
/* XF_Prolog_Move_on.hpp -- Move On                                                  */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Move_on : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Move_on(CardIter* pCI);
   ~XF_Prolog_Move_on(void) noexcept(false);

    static void vXF_Move_on(CardIter* pCI);

    void vXF_Move_on(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Move_on* _pXF_Prolog_Move_on(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

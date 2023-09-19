/*============================================================================*/
/* XF_Prolog_Reduce_not.hpp -- Reduce Not expression ops                      */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Reduce_not : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Reduce_not(CardIter* pCI);
   ~XF_Prolog_Reduce_not(void) noexcept(false);

    static void vProlog(CardIter* pCI);

    void vXF_not_Ops(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Reduce_not* _pXF_Prolog_Reduce_not(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

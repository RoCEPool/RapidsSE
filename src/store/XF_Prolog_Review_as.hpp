/*============================================================================*/
/* XF_Prolog_Review_as.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_Review_as : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_Review_as(CardIter* pCI);
   ~XF_Prolog_Review_as(void) noexcept(false);

    static void vProlog(CardIter* pCI);

    void vXF_as_Review(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_Review_as* _pXF_Prolog_Review_as(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

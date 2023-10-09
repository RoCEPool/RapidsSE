/*============================================================================*/
/* XF_Prolog_as_Review.hpp -- As Review                                              */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_as_Review : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_as_Review(CardIter* pCI);
   ~XF_Prolog_as_Review(void) noexcept(false);

    static void vProlog(CardIter* pCI);

    void vXF_as_Review(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_as_Review* _pXF_Prolog_as_Review(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

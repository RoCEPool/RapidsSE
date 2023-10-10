/*============================================================================*/
/* XF_Epilog_Promote_filter.hpp -- Individuate filter                            */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Promote_filter : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    XF_Epilog_Promote_filter(CardIter* pCI);
   ~XF_Epilog_Promote_filter(void) noexcept(false);

    static void vPromote(CardIter* pCI);

    void vPromote_filter(void);

  private:
    Footer* aFooter(void);

  };

  XF_Epilog_Promote_filter* _pXF_Epilog_Promote_filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

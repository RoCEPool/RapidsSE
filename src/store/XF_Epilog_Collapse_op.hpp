/*============================================================================*/
/* XF_Epilog_Collapse_op.hpp -- Column Required                          */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Collapse_op : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Collapse_op(CardIter* pCI);
   ~XF_Epilog_Collapse_op(void) noexcept(false);

    static void vEpilog(CardIter* pCI);

    void vCollapse_op(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Collapse_op* _pXF_Epilog_Collapse_op(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

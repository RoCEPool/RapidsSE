/*============================================================================*/
/* XF_Epilog_Columns_required.hpp -- Columns required                         */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Columns_required : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Columns_required(CardIter* pCI);
   ~XF_Epilog_Columns_required(void) noexcept(false);

    static void vEpilog(CardIter* pCI);

    void vXF_Columns_required(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Columns_required* _pXF_Epilog_Columns_required(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

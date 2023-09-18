/*============================================================================*/
/* XF_Epilog_Itemise_Columns.hpp -- Itemise column names Cn_Abc                 */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Itemise_Columns : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Itemise_Columns(CardIter* pCI);
   ~XF_Epilog_Itemise_Columns(void) noexcept(false);

    static void vEpilog(CardIter* pCI);

    void vItemise(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Itemise_Columns* _pXF_Epilog_Itemise_Columns(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

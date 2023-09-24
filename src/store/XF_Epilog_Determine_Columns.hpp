/*============================================================================*/
/* XF_Epilog_Determine_Columns.hpp -- Determine Columns (required)            */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Determine_Columns : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Determine_Columns(CardIter* pCI);
   ~XF_Epilog_Determine_Columns(void) noexcept(false);

    static void vDetermine(CardIter* pCI);

    void vDeterime(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Determine_Columns* _pXF_Epilog_Determine_Columns(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

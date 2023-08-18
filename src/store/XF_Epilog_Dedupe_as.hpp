/*============================================================================*/
/* XF_Epilog_Dedupe_as.hpp -- De-Duplicate As (column name)                          */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Epilog_Dedupe_as : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Dedupe_as(CardIter* pCI);
   ~XF_Epilog_Dedupe_as(void) noexcept(false);

    static void vXF_Dedupe_as(CardIter* pCI);

    void vXF_Dedupe_as(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Dedupe_as* _pXF_Epilog_Dedupe_as(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

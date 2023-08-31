/*============================================================================*/
/* XF_Epilog_Itemise_as.hpp -- De-Duplicate As (column name)                          */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Itemise_as : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Itemise_as(CardIter* pCI);
   ~XF_Epilog_Itemise_as(void) noexcept(false);

    static void vEpilog(CardIter* pCI);

    void vEpilog(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Itemise_as* _pXF_Epilog_Itemise_as(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

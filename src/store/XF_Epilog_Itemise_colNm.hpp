/*============================================================================*/
/* XF_Epilog_Itemise_colNm.hpp -- Itemise column names Cn_Abc                 */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Itemise_colNm : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Itemise_colNm(CardIter* pCI);
   ~XF_Epilog_Itemise_colNm(void) noexcept(false);

    static void vEpilog(CardIter* pCI);

    void vEpilog(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Itemise_colNm* _pXF_Epilog_Itemise_colNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

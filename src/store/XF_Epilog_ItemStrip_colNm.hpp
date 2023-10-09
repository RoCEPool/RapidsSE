/*============================================================================*/
/* XF_Epilog_ItemStrip_colNm.hpp -- Strip itemise prefix off col names Cn_Abc */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_ItemStrip_colNm : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_ItemStrip_colNm(CardIter* pCI);
   ~XF_Epilog_ItemStrip_colNm(void) noexcept(false);

    static void vEpilog(CardIter* pCI);

    void vEpilog(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_ItemStrip_colNm* _pXF_Epilog_ItemStrip_colNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

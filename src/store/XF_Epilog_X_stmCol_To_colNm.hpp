/*============================================================================*/
/* XF_Epilog_X_stmCol_To_colNm.hpp -- Stream-Column to Column-Name                     */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_X_stmCol_To_colNm : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_X_stmCol_To_colNm(CardIter* pCI);
   ~XF_Epilog_X_stmCol_To_colNm(void) noexcept(false);

    static void vConvert(CardIter* pCI);

    void vConvert(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_X_stmCol_To_colNm* _pXF_Epilog_X_stmCol_To_colNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

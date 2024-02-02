/*============================================================================*/
/* XF_Prolog_X_colNm_To_expCol.hpp -- Column-Name to Stream-Column                     */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_X_colNm_To_expCol : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_X_colNm_To_expCol(CardIter* pCI);
   ~XF_Prolog_X_colNm_To_expCol(void) noexcept(false);

    static void vXF_colNm_To_expCol(CardIter* pCI);

    void vXF_colNm_To_expCol(void);

  private:

    void vX_colNm_To_expCol(void);

    Footer* aFooter(void);
  };

  XF_Prolog_X_colNm_To_expCol* _pXF_Prolog_X_colNm_To_expCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

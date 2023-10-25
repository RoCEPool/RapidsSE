/*============================================================================*/
/* XF_Prolog_X_colNm_By_stmCol.hpp -- Replace Column-Name by Stream-Column    */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_X_colNm_By_stmCol : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_X_colNm_By_stmCol(CardIter* pCI);
   ~XF_Prolog_X_colNm_By_stmCol(void) noexcept(false);

    static void vXF_colNm_By_stmCol(CardIter* pCI);

    void vXF_colNm_By_stmCol(void);

  private:

    void vX_colNm_By_stmCol(void);

    Footer* aFooter(void);
  };

  XF_Prolog_X_colNm_By_stmCol* _pXF_Prolog_X_colNm_By_stmCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

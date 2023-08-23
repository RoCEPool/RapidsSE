/*============================================================================*/
/* XF_Prolog_colNm_To_stmCol.hpp -- Column-Name to Stream-Column                     */
/*============================================================================*/
#pragma once

#include "XF_PROLOG.hpp"

namespace STORE
{
  class XF_Prolog_colNm_To_stmCol : public XF_PROLOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Prolog_colNm_To_stmCol(CardIter* pCI);
   ~XF_Prolog_colNm_To_stmCol(void) noexcept(false);

    static void vXF_colNm_To_stmCol(CardIter* pCI);

    void vXF_colNm_To_stmCol(void);

  private:
    Footer* aFooter(void);
  };

  XF_Prolog_colNm_To_stmCol* _pXF_Prolog_colNm_To_stmCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

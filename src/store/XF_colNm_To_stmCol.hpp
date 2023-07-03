/*============================================================================*/
/* XF_colNm_to_stmCol.hpp -- Column-Name to Stream-Column                     */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_colNm_to_stmCol : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_colNm_to_stmCol(CardIter* pCI);
   ~XF_colNm_to_stmCol(void) noexcept(false);

    static void vXF_colNm_to_stmCol(CardIter* pCI);

    void vXF_colNm_to_stmCol(void);

  private:
    Footer* aFooter(void);
  };

  XF_colNm_to_stmCol* _pXF_colNm_to_stmCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

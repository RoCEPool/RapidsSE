/*============================================================================*/
/* XF_stmCol_To_colNm.hpp -- Stream-Column to Column-Name                     */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_stmCol_To_colNm : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_stmCol_To_colNm(CardIter* pCI);
   ~XF_stmCol_To_colNm(void) noexcept(false);

    static void vXF_stmCol_To_colNm(CardIter* pCI);

    void vXF_stmCol_To_colNm(void);

  private:
    Footer* aFooter(void);
  };

  XF_stmCol_To_colNm* _pXF_stmCol_To_colNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* XF_stmCol_To_tblCol.hpp -- StmCol to TblCol                                 */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_stmCol_To_tblCol : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_stmCol_To_tblCol(CardIter* pCI);
   ~XF_stmCol_To_tblCol(void) noexcept(false);

    static void vXF_stmCol_To_tblCol(CardIter* pCI);

    void vXF_stmCol_To_tblCol(void);

  private:
    Footer* aFooter(void);
  };

  XF_stmCol_To_tblCol* _pXF_stmCol_To_tblCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

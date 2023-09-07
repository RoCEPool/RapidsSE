/*============================================================================*/
/* XF_Epilog_tblCol_To_stmCol.hpp -- Table-Column to Stream-Column                   */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_tblCol_To_stmCol : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_tblCol_To_stmCol(CardIter* pCI);
   ~XF_Epilog_tblCol_To_stmCol(void) noexcept(false);

    static void vEpilog(CardIter* pCI);

    void vXF_tblCol_To_stmCol(void);

  private:

    void vConvert_tblColOdn_To_stmColOdn(void);
    void vConvert_tblColOdn_To_stmColOdn2(void);

    Footer* aFooter(void);
  };

  XF_Epilog_tblCol_To_stmCol* _pXF_Epilog_tblCol_To_stmCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

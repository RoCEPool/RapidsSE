/*============================================================================*/
/* XF_Epilog_X_tblCol_To_expCol.hpp -- Table-Column to Expression-Column      */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_X_tblCol_To_expCol : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_X_tblCol_To_expCol(CardIter* pCI);
   ~XF_Epilog_X_tblCol_To_expCol(void) noexcept(false);

    static void vConvert(CardIter* pCI);

    void vX_tblCol_To_expCol(void);

  private:

    void vTo_stmCol(void);

    Footer* aFooter(void);
  };

  XF_Epilog_X_tblCol_To_expCol* _pXF_Epilog_X_tblCol_To_expCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* XF_Epilog_Hash_Columns.hpp -- Column Mapping                                    */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Hash_Columns : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Hash_Columns(CardIter* pCI);
   ~XF_Epilog_Hash_Columns(void) noexcept(false);

    static void vHash(CardIter* pCI);

    void vHash(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Hash_Columns* _pXF_Epilog_Hash_Columns(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

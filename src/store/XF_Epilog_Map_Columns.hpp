/*============================================================================*/
/* XF_Epilog_Map_Columns.hpp -- Column Mapping                                    */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Map_Columns : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Map_Columns(CardIter* pCI);
   ~XF_Epilog_Map_Columns(void) noexcept(false);

    static void vMap(CardIter* pCI);

    void vMap(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Map_Columns* _pXF_Epilog_Map_Columns(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

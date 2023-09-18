/*============================================================================*/
/* XF_Epilog_Add_cast.hpp -- Stage Cast                                            */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Add_cast : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Add_cast(CardIter* pCI);
   ~XF_Epilog_Add_cast(void) noexcept(false);

    static void vEpilog(CardIter* pCI);

    void vXF_stage_cast(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Add_cast* _pXF_Epilog_Add_cast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

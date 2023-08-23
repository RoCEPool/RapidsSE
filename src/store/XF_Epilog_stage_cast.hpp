/*============================================================================*/
/* XF_Epilog_stage_cast.hpp -- Stage Cast                                            */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_stage_cast : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_stage_cast(CardIter* pCI);
   ~XF_Epilog_stage_cast(void) noexcept(false);

    static void vXF_stage_cast(CardIter* pCI);

    void vXF_stage_cast(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_stage_cast* _pXF_Epilog_stage_cast(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

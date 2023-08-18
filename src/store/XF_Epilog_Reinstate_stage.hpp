/*============================================================================*/
/* XF_Epilog_Reinstate_stage.hpp -- Reinstate Stage                                  */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Epilog_Reinstate_stage : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Reinstate_stage(CardIter* pCI);
   ~XF_Epilog_Reinstate_stage(void) noexcept(false);

    static void vXF_Reinstate_stage(CardIter* pCI);

    void vXF_Reinstate_stage(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Reinstate_stage* _pXF_Epilog_Reinstate_stage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

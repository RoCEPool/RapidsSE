/*============================================================================*/
/* XF_Epilog_Cull_facet.hpp -- Fuse project                                 */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Cull_facet : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Cull_facet(CardIter* pCI);
   ~XF_Epilog_Cull_facet(void) noexcept(false);

    static void vFuse(CardIter* pCI);

    void vFuse_project(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Cull_facet* _pXF_Epilog_Cull_facet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

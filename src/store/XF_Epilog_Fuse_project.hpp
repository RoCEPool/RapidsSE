/*============================================================================*/
/* XF_Epilog_Fuse_project.hpp -- Fuse project                                 */
/*============================================================================*/
#pragma once

#include "XF_EPILOG.hpp"

namespace STORE
{
  class XF_Epilog_Fuse_project : public XF_EPILOG
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Epilog_Fuse_project(CardIter* pCI);
   ~XF_Epilog_Fuse_project(void) noexcept(false);

    static void vFuse(CardIter* pCI);

    void vFuse_project(void);
    void vFuse_project__(void);

  private:
    Footer* aFooter(void);
  };

  XF_Epilog_Fuse_project* _pXF_Epilog_Fuse_project(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

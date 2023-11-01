/*============================================================================*/
/* XF_Flatten_Purge_where.hpp -- Delete Empty Where                            */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_Purge_where : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Purge_where(CardIter* pCI);
   ~XF_Flatten_Purge_where(void) noexcept(false);

    static void vXF_Delete_Empty_where(CardIter* pCI);

    void vXF_Delete_Empty_where(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Purge_where* _pXF_Flatten_Purge_where(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

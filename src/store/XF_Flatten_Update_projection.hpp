/*============================================================================*/
/* XF_Flatten_Update_projection.hpp -- Update <projection>                            */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Flatten_Update_projection : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Flatten_Update_projection(CardIter* pCI);
   ~XF_Flatten_Update_projection(void) noexcept(false);

    static void vXF_Update_projection(CardIter* pCI);

    void vXF_Update_projection(void);

  private:
    Footer* aFooter(void);
  };

  XF_Flatten_Update_projection* _pXF_Flatten_Update_projection(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

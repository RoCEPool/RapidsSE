/*============================================================================*/
/* XF_Delete_Unneeded.hpp -- Delete Unneeded                                  */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"

namespace STORE
{
  class XF_Delete_Unneeded : public XF_TRANSFORM
  {
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    XF_Delete_Unneeded(CardIter* pCI);
   ~XF_Delete_Unneeded(void) noexcept(false);

    static void vXF_Delete_Unneeded(CardIter* pCI);

    void vXF_Delete_Unneeded(void);

  private:
    Footer* aFooter(void);
  };

  XF_Delete_Unneeded* _pXF_Delete_Unneeded(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

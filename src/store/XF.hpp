/*============================================================================*/
/* XF.hpp -- Transformer                                                      */
/*============================================================================*/
#pragma once

#include "XF_TRANSFORM.hpp"


namespace STORE
{
  class XF : public XF_TRANSFORM
  {
  private:

    CardIter* m_pCI;

    char m_cTableSyn;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF(CardIter* pCI);
   ~XF(void) noexcept(false);

    static void vXF(CardIter* pCI);

    void vTransform(void);
    void vTransform_passThru(void);

    void vSimplify(void);
    void vSimplify_passThru(void);

  private:

  protected:
    Footer* aFooter(void);
  };

  XF* _pXF(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* XF_Flatten_Flatten_pipeline.hpp -- Flatten Pipeline of SubQueries          */
/*============================================================================*/
#pragma once

#include "XF_FLATTEN.hpp"


namespace STORE
{
  class XF_Flatten_Flatten_pipeline : public XF_FLATTEN
  {
  private:

    CardIter* m_pCI;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Flatten_Flatten_pipeline(CardIter* pCI);
   ~XF_Flatten_Flatten_pipeline(void) noexcept(false);

    static void vFlatten(CardIter* pCI);

    void vFlatten(void);

  private:

    void vFlatten_recurse(Symbol* pSym_sqyNo_inner, i8 iDepth);


  protected:
    Footer* aFooter(void);
  };

  XF_Flatten_Flatten_pipeline* _pXF_Flatten_Flatten_pipeline(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Permutor.hpp -- permutor tool                                              */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Permutor : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Permutor(Bench* pBench);
   ~Permutor(void) noexcept(false);

    void vCompute_coPtnList(void);
    void vExtract_tableList(void);
    void vExtract_joinPredList(void);

  private:

    Footer* aFooter(void);
  };

  Permutor* _pPermutor(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

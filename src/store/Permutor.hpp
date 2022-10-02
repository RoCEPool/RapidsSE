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

    void vExtract_tableList(void);

    void vExtract_joinPredList(void);

    void vCompute_coPtnPredList(void);

    void vCompute_coPtnJoinList(void);

    void vCompute_crossPtnJoinList(void);

    void vCompute_ptndJointSet(void);


  private:

    Footer* aFooter(void);
  };

  Permutor* _pPermutor(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

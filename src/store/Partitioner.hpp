/*============================================================================*/
/* Partitioner.hpp -- Partitioner tool                                              */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Partitioner : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Partitioner(Bench* pBench);
   ~Partitioner(void) noexcept(false);

    void vExtract_tableList_From_queryBoard(void);

    void vExtract_joinPredList(void);

    void vCompute_coPtnPredList(void);

    void vCompute_coPtnJoinList(void);

    void vCompute_crossPtnJoinList(void);

    void vCompute_ptndJoinSet(void);


  private:

    Footer* aFooter(void);
  };

  Partitioner* _pPartitioner(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

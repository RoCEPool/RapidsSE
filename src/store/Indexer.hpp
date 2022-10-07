/*============================================================================*/
/* Indexer.hpp -- indexing tool                                               */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Indexer : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Indexer(Bench* pBench);
   ~Indexer(void) noexcept(false);

    void vAppend_indexList_To_scan(void);
    void vMark_scan_pred_bounds(void);
    void vMark_scan_pred_ordinals(void);

  private:

    Footer* aFooter(void);
  };

  Indexer* _pIndexer(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* R_ScanToIndexScan.hpp -- Scan-To-IndexScan Rule                            */
/*============================================================================*/
#pragma once

#include "Rule.hpp"

namespace STORE
{
  class R_ScanToIndexScan : public Rule
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    R_ScanToIndexScan(void);
   ~R_ScanToIndexScan(void) noexcept(false);
  };

  R_ScanToIndexScan* _pR_ScanToIndexScan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

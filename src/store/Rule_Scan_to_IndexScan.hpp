/*============================================================================*/
/* Rule_Scan_to_IndexScan.hpp -- Scan to IndexScan Rule                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Rule.hpp"

namespace STORE
{
  class Rule_Scan_to_IndexScan : public Rule
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Rule_Scan_to_IndexScan(void);
   ~Rule_Scan_to_IndexScan(void) noexcept(false);
  };

  Rule_Scan_to_IndexScan* _pRule_Scan_to_IndexScan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Scanning.hpp -- scan tool                                                    */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Scanning : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Scanning(Bench* pBench);
   ~Scanning(void) noexcept(false);

    void vAdd_scanPred(void);
    void vCull_markedPred(void);
    void vPromote_scan_To_indexScan(void);

  private:

    Footer* aFooter(void);
  };

  Scanning* _pScanning(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

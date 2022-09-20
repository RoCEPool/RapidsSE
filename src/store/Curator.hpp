/*============================================================================*/
/* Curator.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Curator : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Curator(Bench* pBench);
  
   ~Curator(void) noexcept(false);

    void vDelete_Combines(void);
    void vDelete_Sends(void);
    void vLinearize_Stages(void);
    void vReadd_Sends(void);

  private:

    Footer* aFooter(void);
  };

  Curator* _pCurator(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

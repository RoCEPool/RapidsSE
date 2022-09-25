/*============================================================================*/
/* Stager.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Stager : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Stager(Bench* pBench);
   ~Stager(void) noexcept(false);

    void vAdd_Stages(void);
    void vLinearize_Stages(void);

  private:

    Footer* aFooter(void);
  };

  Stager* _pStager(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

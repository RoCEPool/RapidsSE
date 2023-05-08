/*============================================================================*/
/* PipelineDef.hpp -- Pipeline Metadata                                      */
/*============================================================================*/
#pragma once

#include "Meta.hpp"
#include "StageDef.hpp"

namespace STORE
{
  class PipelineDef : public ListNL_t<StageDef>
  {
  protected:

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PipelineDef(void);

    ~PipelineDef(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  PipelineDef* _pPipelineDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
/*============================================================================*/
/* PipelineDef.hpp -- Pipeline Metadata                                      */
/*============================================================================*/
#pragma once

#include "StageDef.hpp"
#include "ListExo2_t.hpp"

namespace STORE
{
  class PipelineDef : public Object
                    , public ListExo2_t<StageDef>
  {
  public:
    s32  m_sPipelineNm;

    s48K m_sCodeBlock;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PipelineDef(void);

    ~PipelineDef(void) noexcept(false);

    void vDump(i8 i_Tab=0);

    Stz* szPipelineNm(void);
    void vPipelineNm(Stz* szPipelineNm);

  protected:
    Footer* aFooter(void);
  };

  PipelineDef* _pPipelineDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
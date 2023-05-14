/*============================================================================*/
/* StageDef.hpp -- Stage Metadata                                            */
/*============================================================================*/
#pragma once

#include "OpDef.hpp"
#include "ListExo2_t.hpp"

namespace STORE
{
  class StageDef : public Object
                 , public ListExo2_t<OpDef>
  {
  public:
    i8  m_i_Stage;
    s32 m_sStageNm;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    StageDef(void);
    ~StageDef(void) noexcept(false);

    void vDump(void);

    i8 i_Stage(void);
    void vStage(i8 i_Stage);

    Stz* szStageNm(void);
    void vStageNm(Stz* szStageNm);

  protected:
    Footer* aFooter(void);
  };

  StageDef* _pStageDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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


  protected:
    Footer* aFooter(void);
  };

  StageDef* _pStageDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* OpDef.hpp -- Op Metadata                                                  */
/*============================================================================*/
#pragma once

#include "ArgDef.hpp"

namespace STORE
{
  class OpDef : public ListNL_t<ArgDef>
  {
  public:
    i8  m_i_Op;
    s32 m_sOpNm;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    OpDef(void);

    ~OpDef(void) noexcept(false);


  protected:
    Footer* aFooter(void);
  };

  OpDef* _pOpDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

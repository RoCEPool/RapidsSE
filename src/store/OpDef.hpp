/*============================================================================*/
/* OpDef.hpp -- Op Metadata                                                  */
/*============================================================================*/
#pragma once

#include "ArgDef.hpp"
#include "IndexDef.hpp"
#include "ColsReq.hpp"
#include "StreamDef.hpp"
#include "IntStack.hpp"

namespace STORE
{
  class OpDef : public ListNL_t<ArgDef>
  {
  public:
    i8  m_i_Op;
    s32 m_sOpNm;

    StreamDef* m_pStreamDef_arriving;
    StreamDef* m_pStreamDef_emitted;

    IndexDef*  m_pIndexDef_join;
    IndexDef*  m_pIndexDef_gbyAgg;

    IntStack*  m_pExprStack;
    ColsReq*   m_pColsReq;

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

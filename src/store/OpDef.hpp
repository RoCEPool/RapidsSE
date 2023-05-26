/*============================================================================*/
/* OpDef.hpp -- Op Metadata                                                  */
/*============================================================================*/
#pragma once

#include "ArgDef.hpp"
#include "IndexDef.hpp"
#include "ColsReq.hpp"
#include "StreamDef.hpp"
#include "IntStack.hpp"
#include "ListExo2_t.hpp"

namespace STORE
{
  class OpDef : public Object
              , public ListExo2_t<ArgDef>
  {
  public:
    i8  m_i_Op;
    s32 m_sOpNm;

    StreamDef* m_pStreamDef_arriving;
    StreamDef* m_pStreamDef_emitted;

    Index*     m_pIndex_indexScan;
    Index*     m_pIndex_indexJoin;

    IndexDef*  m_pIndexDef_gbyAgg;
    IndexDef*  m_pIndexDef_sort;

    ColsReq*   m_pColsReq;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    OpDef(void);
    OpDef(i8 i_Op, Stz* szOpNm);
    ~OpDef(void) noexcept(false);

    ArgDef* pArgDef(i8 i_Arg);
    ArgDef* pArgDef(Stz* szArgNm);
    ArgDef* pArgDef_expr(void);

    ColsReq* pColsReq(void);
    void vColsReq(ColsReq* ColsReq);

    void vDump(i8 i_Tab=2);

    i8 i_Op(void);
    void vOp(i8 i_Op);

    Stz* szOpNm(void);
    void vOpNm(Stz* szOpNm);


    StreamDef* pStreamDef_arriving(void);
    void vStreamDef_arriving(StreamDef* pStreamDef_arriving);

    StreamDef* pStreamDef_emitted(void);
    void vStreamDef_emitted(StreamDef* pStreamDef_emitted);

  protected:
    Footer* aFooter(void);
  };

  OpDef* _pOpDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

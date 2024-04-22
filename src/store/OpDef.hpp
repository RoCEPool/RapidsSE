/*============================================================================*/
/* OpDef.hpp -- Op Metadata                                                  */
/*============================================================================*/
#pragma once

#include "ArgDef.hpp"
#include "IndexDef.hpp"
#include "ColHsh.hpp"
#include "ColMap.hpp"
#include "ColInj.hpp"
#include "ColPpg.hpp"
#include "KeyMap.hpp"
#include "StreamDef.hpp"
#include "IntStack.hpp"
#include "ListExo2_t.hpp"

namespace STORE
{
  class OpDef : public Object
              , public ListExo2_t<ArgDef>
  {
  public:
    i8   m_i_Op;
    s32  m_sOpNm;
    char md_Coord[256];

    s32  m_sAlt;

    Card* m_pCard;

    StreamDef* m_pStreamDef_arriving;
    StreamDef* m_pStreamDef_emitted;

    Index*     m_pIndex_indexScan;
    Index*     m_pIndex_indexJoin;
    Index*     m_pIndex_indexSemi;

    IndexDef*  m_pIndexDef_gbyAgg;
    ColMap*    m_pColMap_gbyAgg;

    IndexDef*  m_pIndexDef_sort;
    ColMap*    m_pColMap_sort;

    IndexDef*  m_pIndexDef_top;
    ColMap*    m_pColMap_top;

    ColPpg*    m_pColPpg;
    ColInj*    m_pColInj;

    ColHsh*    m_pColHsh;
    KeyMap*    m_pKeyMap;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

     OpDef(i8 i_Op, Card* pCard);
    ~OpDef(void) noexcept(false);

    i8      nArgs(void);
    ArgDef* pArgDef(i8 i_Arg);
    ArgDef* pArgDef(Stz* szArgNm);
    ArgDef* pArgDef_expectingVal(Stz* szArgNm);
    ArgDef* pArgDef_expr(void);
    ArgDef* pArgDef_constraint(void);
    ArgDef* pArgDef_count(void);

    ColInj* pColInj(void);
    void vColInj(ColInj* pColInj);

    ColPpg* pColPpg(void);
    void vColPpg(ColPpg* pColPpg);

    void vDump(i8 i_Tab=2);

    i8 i_Op(void);
    void vOp(i8 i_Op);

    Stz* szOpNm(void);
    void vOpNm(Stz* szOpNm);

    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  OpDef* _pOpDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

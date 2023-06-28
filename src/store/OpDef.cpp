/*============================================================================*/
/* OpDef.cpp -- Op Metadata                                                  */
/*============================================================================*/

#include "OpDef.hpp"
#include "Index.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,OpDef,CN_OPDEF)
__aNewFunction(OpDef,CN_OPDEF)
__vDeleteFunction(OpDef,CN_OPDEF)
__aFooterFunction(OpDef)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpDef::OpDef(void)
    : Object(CN_OPDEF)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  m_i_Op = C_UNKNOWN;
  m_nCollapses = 0;
  m_bCollapsed = false;

  m_pStreamDef_arriving = nullptr;
  m_pStreamDef_emitted  = nullptr;

  m_pIndex_indexScan    = nullptr;
  m_pIndex_indexJoin    = nullptr;
  m_pIndex_indexSemi    = nullptr;

  m_pIndexDef_gbyAgg    = nullptr;
  m_pColMap_gbyAgg      = nullptr;
  m_pIndexDef_sort      = nullptr;
  m_pColMap_sort        = nullptr;

  m_pColsReq            = nullptr;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OpDef::OpDef(i8 i_Op, Stz* szOpNm)
    : Object(CN_OPDEF)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  vOp(i_Op);
  vOpNm(szOpNm);

  m_pStreamDef_arriving = nullptr;
  m_pStreamDef_emitted  = nullptr;

  m_pIndex_indexScan    = nullptr;
  m_pIndex_indexJoin    = nullptr;
  m_pIndex_indexSemi    = nullptr;

  m_pIndexDef_gbyAgg    = nullptr;
  m_pColMap_gbyAgg      = nullptr;
  m_pIndexDef_sort      = nullptr;
  m_pColMap_sort        = nullptr;

  m_pColsReq            = nullptr;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OpDef::~OpDef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);

  vDeleteAll();

  delete m_pStreamDef_arriving;
  delete m_pStreamDef_emitted;

  delete m_pIndexDef_gbyAgg;
  delete m_pColMap_gbyAgg;
  delete m_pIndexDef_sort;
  delete m_pColMap_sort;

  delete m_pColsReq;
}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
ArgDef* OpDef::pArgDef(i8 i_Arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  return _pArgDef(pGet(i_Arg));

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
ArgDef* OpDef::pArgDef(Stz* szArgNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  i8 nArgs = nItems();
  for (i8 i_Arg = 0; i_Arg < nArgs; i_Arg++) {
    ArgDef *pArgDef = pGet(i_Arg);
    if (pArgDef->szArgNm()->bSame((szArgNm)))
      return pArgDef;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
ArgDef* OpDef::pArgDef_expr(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  return pArgDef("expr"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
ArgDef* OpDef::pArgDef_constraint(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  return pArgDef("constraint"_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
ColsReq* OpDef::pColsReq(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  return m_pColsReq;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OpDef::vColsLoc_gbyAgg(ColsReq *pColsLoc_gbyAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  m_pColMap_gbyAgg = pColsLoc_gbyAgg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
ColsReq* OpDef::pColsLoc_gbyAgg(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  return m_pColMap_gbyAgg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OpDef::vColsReq(ColsReq* pColsReq)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  m_pColsReq = pColsReq;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OpDef::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);

  vTrace("%sOpDef: [%lld] %s\n"_Stz,
         Stz::szTab(i_Tab),m_i_Op,m_sOpNm.szStz());

  i8 nSpaces = 4;
  if (m_pStreamDef_arriving) {
    vTrace("%s%s : StreamDef_arriving %p\n"_Stz,
           Stz::szTab(i_Tab), Stz::szSpaces(nSpaces), m_pStreamDef_arriving);
    m_pStreamDef_arriving->vDump(i_Tab+4);
  }

  if (m_pStreamDef_emitted) {
    vTrace("%s%s : StreamDef_emitted  %p\n"_Stz,
           Stz::szTab(i_Tab), Stz::szSpaces(nSpaces), m_pStreamDef_emitted);
    m_pStreamDef_emitted->vDump(i_Tab+4);
  }

  vTrace("%s%s : Index_indexScan %p\n"_Stz,
         Stz::szTab(i_Tab),Stz::szSpaces(nSpaces), m_pIndex_indexScan);
  if (m_pIndex_indexScan)
    m_pIndex_indexScan->pIndexDef()->vDump(i_Tab+1);
  vTrace("%s%s : Index_indexJoin %p\n"_Stz,
         Stz::szTab(i_Tab),Stz::szSpaces(nSpaces), m_pIndex_indexJoin);
  if (m_pIndex_indexJoin)
    m_pIndex_indexJoin->pIndexDef()->vDump(i_Tab+1);
  vTrace("%s%s : Index_indexSemi %p\n"_Stz,
         Stz::szTab(i_Tab),Stz::szSpaces(nSpaces), m_pIndex_indexSemi);
  if (m_pIndex_indexSemi)
    m_pIndex_indexSemi->pIndexDef()->vDump(i_Tab+1);


  vTrace("%s%s : IndexDef_gbyAgg %p\n"_Stz,
         Stz::szTab(i_Tab),Stz::szSpaces(nSpaces), m_pIndexDef_gbyAgg);
  if (m_pIndexDef_gbyAgg)
    m_pIndexDef_gbyAgg->vDump(i_Tab+1);

  vTrace("%s%s : ColMap_gbyAgg  %p\n"_Stz,
         Stz::szTab(i_Tab), Stz::szSpaces(nSpaces), m_pColMap_gbyAgg);
  if (m_pColMap_gbyAgg)
    m_pColMap_gbyAgg->vDump(i_Tab+1);

  vTrace("%s%s : IndexDef_sort   %p\n"_Stz,
         Stz::szTab(i_Tab),Stz::szSpaces(nSpaces), m_pIndexDef_sort);
  if (m_pIndexDef_sort)
    m_pIndexDef_sort->vDump(i_Tab+1);

  vTrace("%s%s : ColMap_sort     %p\n"_Stz,
         Stz::szTab(i_Tab), Stz::szSpaces(nSpaces), m_pColMap_sort);
  if (m_pColMap_sort)
    m_pColMap_sort->vDump(i_Tab+1);

  vTrace("%s%s : ColsReq         %p\n"_Stz,
         Stz::szTab(i_Tab),Stz::szSpaces(nSpaces), m_pColsReq);
  if (m_pColsReq)
    m_pColsReq->vDump(i_Tab+1);

  // - - - - - - - - - - - - - - - - - - - - - -

  i8 nArgs = nItems();
  for (i8 i_Arg = 0; i_Arg < nArgs; i_Arg++) {
    ArgDef* pArgDef = pGet(i_Arg);

    pArgDef->vDump(i_Tab+1);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 OpDef::i_Op(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);

  return m_i_Op;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OpDef::vOp(i8 i_Op)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);

  m_i_Op = i_Op;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* OpDef::szOpNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);

  return m_sOpNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OpDef::vOpNm(Stz* szOpNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);

  m_sOpNm = szOpNm;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
StreamDef* OpDef::pStreamDef_arriving(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);

  return m_pStreamDef_arriving;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OpDef::vStreamDef_arriving(StreamDef* pStreamDef_arriving)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);

  m_pStreamDef_arriving = pStreamDef_arriving;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
StreamDef* OpDef::pStreamDef_emitted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);

  return m_pStreamDef_emitted;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OpDef::vStreamDef_emitted(StreamDef* pStreamDef_emitted)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);

  m_pStreamDef_emitted = pStreamDef_emitted;
}

/*----------------------------------------------------------------------------*/
/* validate the op def                                                        */
/*----------------------------------------------------------------------------*/
void OpDef::vValidate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);

  RASSERT(StrLib::bIsSqlName(m_sOpNm.azChar()));

  if (m_sOpNm.bSame("AGGREG"_Stz)) {
    vDebugPoint();
  }
  else
  if (m_sOpNm.bSame("CROSSJOIN"_Stz)) {
    vDebugPoint();
  }
  else
  if (m_sOpNm.bSame("FILTER"_Stz)) {
    vDebugPoint();
  }
  else
  if (m_sOpNm.bSame("GBYAGG"_Stz)) {
    vDebugPoint();
  }
  else
  if (m_sOpNm.bSame("HASH"_Stz)) {
    vDebugPoint();
  }
  else
  if (m_sOpNm.bSame("INDEXJOIN"_Stz)) {
    vDebugPoint();
  }
  else
  if (m_sOpNm.bSame("INDEXSCAN"_Stz)) {
    vDebugPoint();
  }
  else
  if (m_sOpNm.bSame("LIMIT"_Stz)) {
    vDebugPoint();
  }
  else
  if (m_sOpNm.bSame("PROJECT"_Stz)) {
    vDebugPoint();
  }
  else
  if (m_sOpNm.bSame("SCAN"_Stz)) {
    vDebugPoint();
  }
  else
  if (m_sOpNm.bSame("SORT"_Stz)) {
    vDebugPoint();
  }
  else
  if (m_sOpNm.bSame("TOP"_Stz)) {
    vDebugPoint();
  }
  else {
    vDebugPoint();
    NEVER_REACH
  }

  if (m_pStreamDef_arriving)
    m_pStreamDef_arriving->vValidate();

  if (m_pStreamDef_emitted)
    m_pStreamDef_emitted->vValidate();

  if (m_pIndexDef_gbyAgg)
    m_pIndexDef_gbyAgg->vValidate();

  if (m_pColMap_gbyAgg)
    m_pColMap_gbyAgg->vValidate();

  if (m_pIndexDef_sort)
    m_pIndexDef_sort->vValidate();

  if (m_pColMap_sort)
    m_pColMap_sort->vValidate();

  if (m_pColsReq)
    m_pColsReq->vValidate();

//  i8 nArgs = nItems();
//  for (i8 i_Arg = 0; i_Arg < nArgs; i_Arg++) {
//    ArgDef* pArgDef = pGet(i_Arg);
//    pArgDef->vDump();
//  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

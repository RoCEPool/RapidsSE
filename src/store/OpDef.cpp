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
OpDef::OpDef(i8 i_Op, Card* pCard)
    : Object(CN_OPDEF)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  vOp(i_Op);
  m_pCard = pCard;

  //vOpNm(szOpNm);

       if (pCard->bIsCard(CK_HAVING)) vOpNm("filter"_Stz);
  else if (pCard->bIsCard(CK_HASH))   vOpNm("hash"_Stz);
  else if (pCard->bIsCard(CK_FROM))   vOpNm("scan"_Stz);
  else if (pCard->bIsCard(CK_WHERE))  vOpNm("filter"_Stz);
  else
    vOpNm(pCard->szCardNm());


  MemLib::vZero(md_Coord, sizeof(md_Coord), CN_OPDEF);
  StrLib::vCopy(md_Coord, sizeof(md_Coord), pCard->szCoord()->azChar());

  m_pStreamDef_arriving = nullptr;
  m_pStreamDef_emitted  = nullptr;

  m_pIndex_indexScan    = nullptr;
  m_pIndex_indexJoin    = nullptr;
  m_pIndex_indexSemi    = nullptr;

  m_pIndexDef_gbyAgg    = nullptr;
  m_pColMap_gbyAgg      = nullptr;

  m_pIndexDef_sort      = nullptr;
  m_pColMap_sort        = nullptr;

  m_pIndexDef_top       = nullptr;
  m_pColMap_top         = nullptr;

  m_pColPpg             = nullptr;
  m_pColInj             = nullptr;

  m_pColHsh             = nullptr;

  _checkInitialised_(CN_OPDEF);

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
  delete m_pIndexDef_top;
  delete m_pColMap_top;

  delete m_pColPpg;
  delete m_pColInj;
  delete m_pColHsh;
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
i8 OpDef::nArgs(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  return nItems();
}

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
/* get arg def by name (provided it has a value)                              */
/*----------------------------------------------------------------------------*/
ArgDef* OpDef::pArgDef_expectingVal(Stz* szArgNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  ArgDef *pArgDef = this->pArgDef(szArgNm);
  if (! pArgDef)
    return nullptr;

  Stz* szArgVal = pArgDef->szArgVal();
  if (! szArgVal)
    return nullptr;

  if (StrLib::bIsEmpty(szArgVal->azChar()) ||
      StrLib::bIsWhiteSpaces(szArgVal->azChar()))
    return nullptr;

  return pArgDef;
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
ArgDef* OpDef::pArgDef_count(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  return pArgDef("count"_Stz);
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
ColInj* OpDef::pColInj(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  return m_pColInj;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OpDef::vColInj(ColInj* pColInj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  m_pColInj = pColInj;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
ColPpg* OpDef::pColPpg(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  return m_pColPpg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OpDef::vColPpg(ColPpg* pColPpg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  m_pColPpg = pColPpg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void OpDef::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_OPDEF, _FOOTER_);

  i8 nTab_plus = 4;
  vTrace("%sOpDef: [%lld] %s %s\n"_Stz,
         Stz::szTab(i_Tab),
         m_i_Op,
         m_sOpNm.szStz(),
         md_Coord);

  i8 nSpaces = 4;
  if (m_pStreamDef_arriving) {
    vTrace("%s%s : StreamDef_arriving %p\n"_Stz,
           Stz::szTab(i_Tab), Stz::szSpaces(nSpaces), m_pStreamDef_arriving);
    m_pStreamDef_arriving->vDump(i_Tab + 4);
  }

  if (m_pStreamDef_emitted) {
    vTrace("%s%s : StreamDef_emitted %p\n"_Stz,
           Stz::szTab(i_Tab), Stz::szSpaces(nSpaces), m_pStreamDef_emitted);
    m_pStreamDef_emitted->vDump(i_Tab + 4);
  }

  if (m_pIndex_indexScan) {
    vTrace("%s%s : Index_indexScan %p\n"_Stz,
           Stz::szTab(i_Tab), Stz::szSpaces(nSpaces), m_pIndex_indexScan);
    m_pIndex_indexScan->pIndexDef()->vDump(i_Tab + nTab_plus);
  }
  if (m_pIndex_indexJoin) {
    vTrace("%s%s : Index_indexJoin %p\n"_Stz,
           Stz::szTab(i_Tab),Stz::szSpaces(nSpaces), m_pIndex_indexJoin);
    m_pIndex_indexJoin->pIndexDef()->vDump(i_Tab + nTab_plus);
  }
  if (m_pIndex_indexSemi) {
    vTrace("%s%s : Index_indexSemi %p\n"_Stz,
           Stz::szTab(i_Tab),Stz::szSpaces(nSpaces), m_pIndex_indexSemi);
    m_pIndex_indexSemi->pIndexDef()->vDump(i_Tab + nTab_plus);
  }

  if (m_pIndexDef_gbyAgg) {
    vTrace("%s%s : IndexDef_gbyAgg %p\n"_Stz,
           Stz::szTab(i_Tab),Stz::szSpaces(nSpaces), m_pIndexDef_gbyAgg);
    m_pIndexDef_gbyAgg->vDump(i_Tab + nTab_plus);
  }

  if (m_pColMap_gbyAgg) {
    vTrace("%s%s : ColMap_gbyAgg %p\n"_Stz,
           Stz::szTab(i_Tab), Stz::szSpaces(nSpaces), m_pColMap_gbyAgg);
    m_pColMap_gbyAgg->vDump(i_Tab + nTab_plus);
  }

  if (m_pIndexDef_sort) {
    vTrace("%s%s : IndexDef_sort %p\n"_Stz,
           Stz::szTab(i_Tab),Stz::szSpaces(nSpaces), m_pIndexDef_sort);
    m_pIndexDef_sort->vDump(i_Tab + nTab_plus);
  }

  if (m_pColMap_sort) {
    vTrace("%s%s : ColMap_sort %p\n"_Stz,
           Stz::szTab(i_Tab), Stz::szSpaces(nSpaces), m_pColMap_sort);
    m_pColMap_sort->vDump(i_Tab + nTab_plus);
  }

  if (m_pIndexDef_top) {
    vTrace("%s%s : IndexDef_top %p\n"_Stz,
           Stz::szTab(i_Tab),Stz::szSpaces(nSpaces), m_pIndexDef_top);
    m_pIndexDef_top->vDump(i_Tab + nTab_plus);
  }

  if (m_pColMap_top) {
    vTrace("%s%s : ColMap_top %p\n"_Stz,
           Stz::szTab(i_Tab), Stz::szSpaces(nSpaces), m_pColMap_top);
    m_pColMap_top->vDump(i_Tab + 1);
  }
  
  if (m_pColPpg) {
    vTrace("%s%s : ColPpg %p\n"_Stz,
           Stz::szTab(i_Tab), Stz::szSpaces(nSpaces), m_pColPpg);
    m_pColPpg->vDump(i_Tab + 1);
  }

  if (m_pColInj) {
    vTrace("%s%s : ColInj %p\n"_Stz,
           Stz::szTab(i_Tab), Stz::szSpaces(nSpaces), m_pColInj);
    m_pColInj->vDump(i_Tab + 1);
  }

  if (m_pColHsh) {
    vTrace("%s%s : ColHsh %p\n"_Stz,
           Stz::szTab(i_Tab),Stz::szSpaces(nSpaces), m_pColHsh);
    m_pColHsh->vDump(i_Tab + 1);
  }

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
/* validate the op def                                                        */
/*----------------------------------------------------------------------------*/
void OpDef::vValidate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OPDEF,_FOOTER_);

  RASSERT(StrLib::bIsSqlName(m_sOpNm.azChar()));

  if (m_sOpNm.bSame("AGGREG"_Stz)) {
    RASSERT(1 == nArgs());
    RASSERT(pArgDef("expr"_Stz));

    RASSERT(m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
  }
  else
  if (m_sOpNm.bSame("CROSSJOIN"_Stz)) {
    RASSERT(3 == nArgs());
    RASSERT(pArgDef("tableNm"_Stz));
    RASSERT(pArgDef("indexNm"_Stz));
    RASSERT(pArgDef("expr"_Stz));

    RASSERT(m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
    RASSERT(m_pIndex_indexJoin);
    RASSERT(m_pColInj);
  }
  else
  if (m_sOpNm.bSame("FILTER"_Stz)) {
    RASSERT(1 == nArgs());
    RASSERT(pArgDef("expr"_Stz));

    RASSERT(m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
  }
  else
  if (m_sOpNm.bSame("GBYAGG"_Stz)) {
    RASSERT(1 == nArgs());
    RASSERT(pArgDef("expr"_Stz));

    RASSERT(m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
    RASSERT(m_pIndexDef_gbyAgg);
    RASSERT(m_pColMap_gbyAgg);
  }
  else
  if (m_sOpNm.bSame("HASH"_Stz)) {
    RASSERT(1 == nArgs());
    RASSERT(pArgDef("expr"_Stz));

    RASSERT(m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
  }
  else
  if (m_sOpNm.bSame("INDEXJOIN"_Stz)) {
    RASSERT(nArgs() >= 3);
    RASSERT(pArgDef("tableNm"_Stz));
    RASSERT(pArgDef("indexNm"_Stz));
    RASSERT(pArgDef_expectingVal("expr"_Stz));
    RASSERT(nArgs() == 3 || pArgDef_expectingVal("constraint"_Stz));

    RASSERT(m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
    RASSERT(m_pIndex_indexJoin);
    RASSERT(m_pColInj);
  }
  else
  if (m_sOpNm.bSame("INDEXSCAN"_Stz)) {
    RASSERT(3 == nArgs());
    RASSERT(pArgDef("tableNm"_Stz));
    RASSERT(pArgDef("indexNm"_Stz));
    RASSERT(pArgDef("expr"_Stz));

    //RASSERT(! m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
    RASSERT(m_pIndex_indexScan);
    RASSERT(m_pColInj);
  }
  else
  if (m_sOpNm.bSame("INFLATE"_Stz)) {
    RASSERT(1 == nArgs());
    RASSERT(pArgDef("tableNm"_Stz));

    //RASSERT(! m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
    RASSERT(m_pColInj);
  }
  else
  if (m_sOpNm.bSame("KEYEXISTS"_Stz)) {
    RASSERT(4 == nArgs());
//    RASSERT(pArgDef("tableNm"_Stz));
//    RASSERT(pArgDef("indexNm"_Stz));
//    RASSERT(pArgDef("constraint"_Stz));
//    RASSERT(pArgDef("expr"_Stz));

//    RASSERT(! m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
    RASSERT(m_pIndex_indexSemi);
//    RASSERT(m_pColsReq);
  }
  else
  if (m_sOpNm.bSame("KEYNOTEXISTS"_Stz)) {
    RASSERT(4 == nArgs());
//    RASSERT(pArgDef("tableNm"_Stz));
//    RASSERT(pArgDef("indexNm"_Stz));
//    RASSERT(pArgDef("constraint"_Stz));
//    RASSERT(pArgDef("expr"_Stz));

//    RASSERT(! m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
    RASSERT(m_pIndex_indexSemi);
//    RASSERT(m_pColsReq);
  }
  else
  if (m_sOpNm.bSame("LIMIT"_Stz)) {
    RASSERT(1 == nArgs());
    RASSERT(pArgDef("expr"_Stz));

    RASSERT(m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
  }
  else
  if (m_sOpNm.bSame("ORDAGG"_Stz)) {
    RASSERT(1 == nArgs());
    RASSERT(pArgDef("expr"_Stz));

    RASSERT(m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
  }
  else
  if (m_sOpNm.bSame("PROJECT"_Stz)) {
    RASSERT(1 == nArgs());
    RASSERT(pArgDef("expr"_Stz));

    RASSERT(m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
  }
  else
  if (m_sOpNm.bSame("SCAN"_Stz)) {
    RASSERT(1 == nArgs());
    RASSERT(pArgDef("tableNm"_Stz));

    //RASSERT(! m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
    RASSERT(m_pColInj);
  }
  else
  if (m_sOpNm.bSame("SORT"_Stz)) {
    RASSERT(1 == nArgs());
    RASSERT(pArgDef("expr"_Stz));

    RASSERT(m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);
    RASSERT(m_pIndexDef_sort);
    RASSERT(m_pColMap_sort);

    //RASSERT(m_pStreamDef_emitted->nColumns() == m_pStreamDef_arriving->nColumns());
  }
  else
  if (m_sOpNm.bSame("TOP"_Stz)) {
    RASSERT(2 == nArgs());
    RASSERT(pArgDef("expr"_Stz));
    RASSERT(pArgDef("count"_Stz));
    RASSERT(m_pStreamDef_arriving);
    RASSERT(m_pStreamDef_emitted);

    RASSERT(m_pStreamDef_emitted->nColumns() == m_pStreamDef_arriving->nColumns());
  }
  else {
    // an operator is missing
    vDump();
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

  if (m_pIndexDef_top)
    m_pIndexDef_top->vValidate();

  if (m_pColMap_top)
    m_pColMap_top->vValidate();

  if (m_pColInj)
    m_pColInj->vValidate();

//  i8 nArgs = nItems();
//  for (i8 i_Arg = 0; i_Arg < nArgs; i_Arg++) {
//    ArgDef* pArgDef = pGet(i_Arg);
//    pArgDef->vDump();
//  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

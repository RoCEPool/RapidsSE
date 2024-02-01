/*============================================================================*/
/* SubQuery.cpp -- SubQuery analysis                                          */
/*============================================================================*/

#include "SubQuery.hpp"

#include "Board.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, SubQuery, CN_SUBQUERY)
__aNewFunction(SubQuery, CN_SUBQUERY)
__vDeleteFunction(SubQuery, CN_SUBQUERY)
__aFooterFunction(SubQuery)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SubQuery::SubQuery(i8 i_Board, Stz* szSqyNm, Board* pBoard)
: Object(CN_SUBQUERY)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_pBoard = pBoard;
  m_i_Caller = C_UNKNOWN;

  m_eDisposition = QD_NONE;
  m_nCorrCols    = 0;

  m_eClause  = QC_NONE;
  m_eCallOp  = QO_NONE;
  m_eRelnOp  = RO_NONE;

  m_nSubQueries = 0;

  vSqyNm(szSqyNm);
  vSqyNo(szSqyNm);
  UNUSED(i_Board);

  _checkInitialised_(CN_SUBQUERY);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SubQuery::~SubQuery(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get board                                                                  */
/*----------------------------------------------------------------------------*/
Board* SubQuery::pBoard(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_pBoard;
}

/*----------------------------------------------------------------------------*/
/* called by/wrt exists                                                       */
/*----------------------------------------------------------------------------*/
bool SubQuery::bByExists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  if (QO_EXISTS_OP == m_eCallOp)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* called by/wrt in                                                           */
/*----------------------------------------------------------------------------*/
bool SubQuery::bByIn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  if (QO_IN_OP == m_eCallOp)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* called by/wrt not exists                                                   */
/*----------------------------------------------------------------------------*/
bool SubQuery::bByNotExists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  if (QO_NOT_EXISTS_OP == m_eCallOp)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* called by/wrt not in                                                       */
/*----------------------------------------------------------------------------*/
bool SubQuery::bByNotIn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  if (QO_NOT_IN_OP == m_eCallOp)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* called by/wrt relnOp                                                       */
/*----------------------------------------------------------------------------*/
bool SubQuery::bByRelnOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  if (QO_RELN_OP == m_eCallOp)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get i_Caller                                                               */
/*----------------------------------------------------------------------------*/
i8 SubQuery::i_Caller(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_i_Caller;
}

/*----------------------------------------------------------------------------*/
/* set i_Caller                                                               */
/*----------------------------------------------------------------------------*/
void SubQuery::vCaller(i8 i_Caller)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_i_Caller = i_Caller;
}

/*----------------------------------------------------------------------------*/
/* get clause                                                                 */
/*----------------------------------------------------------------------------*/
eQC SubQuery::eClause(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_eClause;
}

/*----------------------------------------------------------------------------*/
/* set clause                                                                 */
/*----------------------------------------------------------------------------*/
void SubQuery::vClause(eQC eClause)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_eClause = eClause;
}

/*----------------------------------------------------------------------------*/
/* get operator                                                               */
/*----------------------------------------------------------------------------*/
eQO SubQuery::eCallOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_eCallOp;
}

/*----------------------------------------------------------------------------*/
/* set operator                                                               */
/*----------------------------------------------------------------------------*/
void SubQuery::vCallOp(eQO eCalcOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_eCallOp = eCalcOp;
}

/*----------------------------------------------------------------------------*/
/* get nCorrCols                                                              */
/*----------------------------------------------------------------------------*/
i8 SubQuery::nCorrCols(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_nCorrCols;
}

/*----------------------------------------------------------------------------*/
/* set nCorrCols                                                              */
/*----------------------------------------------------------------------------*/
void SubQuery::vCorrCols(i8 nCorrCols)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_nCorrCols = nCorrCols;
}

/*----------------------------------------------------------------------------*/
/* get disposition                                                            */
/*----------------------------------------------------------------------------*/
eQD SubQuery::eDisposition(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_eDisposition;
}

/*----------------------------------------------------------------------------*/
/* set disposition                                                            */
/*----------------------------------------------------------------------------*/
void SubQuery::vDisposition(eQD eDisposition)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_eDisposition = eDisposition;
}

/*----------------------------------------------------------------------------*/
/* dump this subquery                                                         */
/*----------------------------------------------------------------------------*/
void SubQuery::vDump(i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  vTrace("%sSubQuery v1 %p SqyNo %s SqyNm %s Caller %lld; %s %s %s %s (%lld nCorrCols)\n"_Stz,
         Stz::szTab(iDepth),
         this,
         szSqyNo(),
         szSqyNm(),
         m_i_Caller,

         aQC[m_eClause],
         aQD[m_eDisposition],
         aQO[m_eCallOp],
         aRO[m_eRelnOp],
         m_nCorrCols
         );

  vTrace("%sSubQuery v2 %p SqyNo %s SqyNm %s Caller %lld; %s %s %s %s (%lld nCorrCols)\n"_Stz,
         Stz::szTab(iDepth),
         this,
         szSqyNo_v2(),
         szSqyNm_v2(),
         m_i_Caller,

         aQC[m_eClause],
         aQD[m_eDisposition],
         aQO[m_eCallOp],
         aRO[m_eRelnOp],
         m_nCorrCols
  );

}


/*----------------------------------------------------------------------------*/
/* is this sub-query an aggegation?                                           */
/*----------------------------------------------------------------------------*/
bool SubQuery::bIsAggregation(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  if (QD_AGGREGATION == m_eDisposition)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this sub-query correlated?                                              */
/*----------------------------------------------------------------------------*/
bool SubQuery::bIsCorrelated(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  if (m_nCorrCols)
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/* is this sub-query linear?                                                  */
/*----------------------------------------------------------------------------*/
bool SubQuery::bIsLinear(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  if (QD_LINEAR == m_eDisposition)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get relational op                                                          */
/*----------------------------------------------------------------------------*/
eRO SubQuery::eRelnOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_eRelnOp;
}

/*----------------------------------------------------------------------------*/
/* set relational op                                                          */
/*----------------------------------------------------------------------------*/
void SubQuery::vRelnOp(eRO eRelnOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_eRelnOp = eRelnOp;
}

/*----------------------------------------------------------------------------*/
/* get subquery name                                                          */
/*----------------------------------------------------------------------------*/
Stz* SubQuery::szSqyNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBQUERY,_FOOTER_);

  return m_sSqyNm;
}

/*----------------------------------------------------------------------------*/
/* get subquery name                                                          */
/*----------------------------------------------------------------------------*/
void SubQuery::vSqyNm(Stz* szSqyNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBQUERY,_FOOTER_);

  if (szSqyNm->bSame("$Q2"_Stz))
    vDebugPoint();

  m_sSqyNm = szSqyNm;
  m_sSqyNo_v2 = szSqyNm;
}

/*----------------------------------------------------------------------------*/
/* get subquery number $Qn                                                    */
/*----------------------------------------------------------------------------*/
Stz* SubQuery::szSqyNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBQUERY,_FOOTER_);

  return m_sSqyNo;
}

/*----------------------------------------------------------------------------*/
/* set subquery number $Qn                                                    */
/*----------------------------------------------------------------------------*/
void SubQuery::vSqyNo(Stz* szSqyNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBQUERY,_FOOTER_);

  if (szSqyNo->bSame("$Q2"_Stz))
    vDebugPoint();

  m_sSqyNo = szSqyNo;
  m_sSqyNm_v2 = szSqyNo;

}

/*----------------------------------------------------------------------------*/
/* get subquery name                                                          */
/*----------------------------------------------------------------------------*/
Stz* SubQuery::szSqyNm_v2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBQUERY,_FOOTER_);

  return m_sSqyNm_v2;
}

/*----------------------------------------------------------------------------*/
/* get subquery name                                                          */
/*----------------------------------------------------------------------------*/
void SubQuery::vSqyNm_v2(Stz* szSqyNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBQUERY,_FOOTER_);

  if (szSqyNm->bSame("$Q2"_Stz))
    vDebugPoint();

  m_sSqyNm_v2 = szSqyNm;
}

/*----------------------------------------------------------------------------*/
/* get subquery number $Qn                                                    */
/*----------------------------------------------------------------------------*/
Stz* SubQuery::szSqyNo_v2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBQUERY,_FOOTER_);

  return m_sSqyNo_v2;
}

/*----------------------------------------------------------------------------*/
/* set subquery number $Qn                                                    */
/*----------------------------------------------------------------------------*/
void SubQuery::vSqyNo_v2(Stz* szSqyNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SUBQUERY,_FOOTER_);

  if (szSqyNo->bSame("$Q2"_Stz))
    vDebugPoint();

  m_sSqyNo_v2 = szSqyNo;
}

/*----------------------------------------------------------------------------*/
/* get nSubQueries                                                            */
/*----------------------------------------------------------------------------*/
i8 SubQuery::nSubQueries(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_nSubQueries;
}

/*----------------------------------------------------------------------------*/
/* set nSubQueries                                                            */
/*----------------------------------------------------------------------------*/
void SubQuery::vSubQueries(i8 nSubQueries)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_nSubQueries = nSubQueries;
}

/*----------------------------------------------------------------------------*/
/* is this sub-query called via a from clause?                                */
/*----------------------------------------------------------------------------*/
bool SubQuery::bViaFrom(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  if (QC_FROM_CLAUSE == m_eClause)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this sub-query called via a having clause?                              */
/*----------------------------------------------------------------------------*/
bool SubQuery::bViaHaving(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  if (QC_HAVING_CLAUSE == m_eClause)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this sub-query called via a where clause?                               */
/*----------------------------------------------------------------------------*/
bool SubQuery::bViaWhere(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  if (QC_WHERE_CLAUSE == m_eClause)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void SubQuery::vValidate(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  RASSERT(pCI->G.pBoard);
  RASSERT(pCI->G.pBoard->pDeck() == pCI->G.pDeck);

  if (! pCI->m_pBench->bPastPhase(pCI, OH_PROLOG))
    return;

  RASSERT(! m_sSqyNo_v2.nLength() || m_sSqyNo_v2.bSame(m_sSqyNm));
  RASSERT(! m_sSqyNm_v2.nLength() || m_sSqyNm_v2.bSame(m_sSqyNo));

  pCI->vSave();
  pCI->vNullMembers();
  pCI->G.pDeck->vValidate(pCI);
  pCI->vRestore();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

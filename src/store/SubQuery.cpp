/*============================================================================*/
/* SubQuery.cpp -- SubQuery analysis                                                */
/*============================================================================*/

#include "SubQuery.hpp"
#include "Card.hpp"

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
SubQuery::SubQuery(void)
: Object(CN_SUBQUERY)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_iForm = C_UNKNOWN;

  m_i_SubQuery = 0;
  m_i_Caller   = 0;

  m_nParams      = 0;
  m_eDisposition = QD_NONE;
  m_eClause      = QC_NONE;
  m_eOperator    = QO_NONE;

  m_eRelationalOp = RO_NONE;

  m_nSubQueries = 0;

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
/* get form                                                                   */
/*----------------------------------------------------------------------------*/
i8 SubQuery::iForm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_iForm;
}

/*----------------------------------------------------------------------------*/
/* set form                                                                   */
/*----------------------------------------------------------------------------*/
void SubQuery::vForm(i8 iForm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_iForm = iForm;
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
eQO SubQuery::eOperator(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_eOperator;
}

/*----------------------------------------------------------------------------*/
/* set operator                                                               */
/*----------------------------------------------------------------------------*/
void SubQuery::vOperator(eQO eOperator)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_eOperator = eOperator;
}

/*----------------------------------------------------------------------------*/
/* get nParams                                                                */
/*----------------------------------------------------------------------------*/
i8 SubQuery::nParams(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_nParams;
}

/*----------------------------------------------------------------------------*/
/* set nParams                                                                */
/*----------------------------------------------------------------------------*/
void SubQuery::vParams(i8 nParams)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_nParams = nParams;
}

/*----------------------------------------------------------------------------*/
/* get relational op                                                          */
/*----------------------------------------------------------------------------*/
eRO SubQuery::eRelationalOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_eRelationalOp;
}

/*----------------------------------------------------------------------------*/
/* set relational op                                                          */
/*----------------------------------------------------------------------------*/
void SubQuery::vRelationalOp(eRO eRelationalOp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_eRelationalOp = eRelationalOp;
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
/* get i_SubQuery                                                             */
/*----------------------------------------------------------------------------*/
i8 SubQuery::i_SubQuery(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_i_SubQuery;
}

/*----------------------------------------------------------------------------*/
/* set i_SubQuery                                                             */
/*----------------------------------------------------------------------------*/
void SubQuery::vSubQuery(i8 i_SubQuery)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_i_SubQuery = i_SubQuery;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

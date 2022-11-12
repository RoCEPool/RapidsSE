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

  m_eAggregation = QA_NONE;
  m_eClause      = QC_NONE;
  m_eOperator    = QO_NONE;

  m_eRelationalOp = RO_NONE;

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
/* get aggregation                                                            */
/*----------------------------------------------------------------------------*/
eQA SubQuery::eAggregation(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  return m_eAggregation;
}

/*----------------------------------------------------------------------------*/
/* set aggregation                                                            */
/*----------------------------------------------------------------------------*/
void SubQuery::vAggregation(eQA eAggregation)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_SUBQUERY, _FOOTER_);

  m_eAggregation = eAggregation;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

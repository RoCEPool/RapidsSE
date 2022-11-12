/*============================================================================*/
/* Query.cpp -- Query analysis                                                */
/*============================================================================*/

#include "Query.hpp"
#include "Card.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Query,CN_QUERY)
__aNewFunction(Query,CN_QUERY)
__vDeleteFunction(Query,CN_QUERY)
__aFooterFunction(Query)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Query::Query(void)
: Object(CN_QUERY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);

  m_iForm = C_UNKNOWN;

  m_eAggregation = QA_NONE;
  m_eClause      = QC_NONE;
  m_eOperator    = QO_NONE;

  m_eRelationalOp = RO_NONE;

  _checkInitialised_(CN_QUERY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Query::~Query(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get form                                                                   */
/*----------------------------------------------------------------------------*/
i8 Query::iForm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);

  return m_iForm;
}

/*----------------------------------------------------------------------------*/
/* set form                                                                   */
/*----------------------------------------------------------------------------*/
void Query::vForm(i8 iForm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);

  m_iForm = iForm;
}

/*----------------------------------------------------------------------------*/
/* get aggregation                                                            */
/*----------------------------------------------------------------------------*/
eQA Query::eAggregation(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);

  return m_eAggregation;
}

/*----------------------------------------------------------------------------*/
/* set aggregation                                                            */
/*----------------------------------------------------------------------------*/
void Query::vAggregation(eQA eAggregation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);

  m_eAggregation = eAggregation;
}

/*----------------------------------------------------------------------------*/
/* get clause                                                                 */
/*----------------------------------------------------------------------------*/
eQC Query::eClause(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);

  return m_eClause;
}

/*----------------------------------------------------------------------------*/
/* set clause                                                                 */
/*----------------------------------------------------------------------------*/
void Query::vClause(eQC eClause)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);

  m_eClause = eClause;
}

/*----------------------------------------------------------------------------*/
/* get operator                                                               */
/*----------------------------------------------------------------------------*/
eQO Query::eOperator(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);

  return m_eOperator;
}

/*----------------------------------------------------------------------------*/
/* set operator                                                               */
/*----------------------------------------------------------------------------*/
void Query::vOperator(eQO eOperator)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);

  m_eOperator = eOperator;
}

/*----------------------------------------------------------------------------*/
/* get relational op                                                          */
/*----------------------------------------------------------------------------*/
eRO Query::eRelationalOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);

  return m_eRelationalOp;
}

/*----------------------------------------------------------------------------*/
/* set relational op                                                          */
/*----------------------------------------------------------------------------*/
void Query::vRelationalOp(eRO eRelationalOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERY,_FOOTER_);

  m_eRelationalOp = eRelationalOp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

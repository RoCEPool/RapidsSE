/*============================================================================*/
/* SqlOperator.hpp - an abstract base class of all objects that process       */
/* streams of row data.                                                       */
/*============================================================================*/

#include "BASE.h"
#include "SqlOperator.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "store/parser/shadowNodes/planObjects/ProjectOp.hpp"
#include "store/parser/shadowNodes/planObjects/AggregateOp.hpp"
#include "store/parser/shadowNodes/planObjects/dataSources/DataSource.hpp"
#include "store/parser/shadowNodes/planObjects/FilterOp.hpp"
#include "store/parser/shadowNodes/planObjects/LimitOp.hpp"
#include "UnionOp.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SQLOPERATOR_DEFN = "/__root__/definitions/object/planObject/sqlOperator"_Stz;

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void SqlOperator::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  if      (AggregateOp::bIsExactType(pNode))   { AggregateOp::vDescribe(pNode, sBuf); }
  else if (  ProjectOp::bIsExactType(pNode))   {   ProjectOp::vDescribe(pNode, sBuf); }
  else if ( DataSource::bIsOfType(pNode))      {  DataSource::vDescribe(pNode, sBuf); }
  else if (   FilterOp::bIsExactType(pNode))   {    FilterOp::vDescribe(pNode, sBuf); }
  else if (    LimitOp::bIsExactType(pNode))   {     LimitOp::vDescribe(pNode, sBuf); }
  else if (    UnionOp::bIsExactType(pNode))   {     UnionOp::vDescribe(pNode, sBuf); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SqlOperator::vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism here
  if      (AggregateOp::bIsExactType(pNode)) { AggregateOp::vExplain(pNode, pRootNode, sBuf, iIndent); }
  else if (  ProjectOp::bIsExactType(pNode)) {   ProjectOp::vExplain(pNode, pRootNode, sBuf, iIndent); }
  else if ( DataSource::bIsOfType(pNode))    {  DataSource::vExplain(pNode, pRootNode, sBuf, iIndent); }
  else if (   FilterOp::bIsExactType(pNode)) {    FilterOp::vExplain(pNode, pRootNode, sBuf, iIndent); }
  else if (    LimitOp::bIsExactType(pNode)) {     LimitOp::vExplain(pNode, pRootNode, sBuf, iIndent); }
  else if (    UnionOp::bIsExactType(pNode)) {     UnionOp::vExplain(pNode, pRootNode, sBuf, iIndent); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 SqlOperator::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SQLOPERATOR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlOperator::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SQLOPERATOR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions      */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
PlanNodeList* SqlOperator::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pRootNode);

  // Manual polymorphism here
  if      (AggregateOp::bIsOfType(pNode)) { return AggregateOp::pColumnExprs(pNode, pRootNode); }
  else if (  ProjectOp::bIsOfType(pNode)) { return   ProjectOp::pColumnExprs(pNode, pRootNode); }
  else if ( DataSource::bIsOfType(pNode)) { return  DataSource::pColumnExprs(pNode, pRootNode); }
  else if (   FilterOp::bIsOfType(pNode)) { return    FilterOp::pColumnExprs(pNode, pRootNode); }
  else if (    LimitOp::bIsOfType(pNode)) { return     LimitOp::pColumnExprs(pNode, pRootNode); }
  else if (    UnionOp::bIsOfType(pNode)) { return     UnionOp::pColumnExprs(pNode, pRootNode); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

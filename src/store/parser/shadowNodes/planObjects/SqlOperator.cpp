/*============================================================================*/
/* SqlOperator.hpp - an abstract base class of all objects that process       */
/* streams of row data.                                                       */
/*============================================================================*/

#include "BASE.h"
#include "SqlOperator.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"

#include "store/parser/shadowNodes/planObjects/ProjectOp.hpp"
#include "store/parser/shadowNodes/planObjects/AggregateOp.hpp"
#include "store/parser/shadowNodes/planObjects/dataSources/DataSource.hpp"
#include "store/parser/shadowNodes/planObjects/FilterOp.hpp"
#include "store/parser/shadowNodes/planObjects/LimitOp.hpp"
#include "store/parser/shadowNodes/planObjects/OrderOp.hpp"
#include "store/parser/shadowNodes/planObjects/ExplainOp.hpp"
#include "store/parser/shadowNodes/planObjects/UnionOp.hpp"
#include "store/parser/shadowNodes/planObjects/joins/JoinOp.hpp"

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
void SqlOperator::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  if      (AggregateOp::bIsExactType(pNode))   { AggregateOp::vDescribe(pNode, sBuf, pRootNode); }
  else if (  ProjectOp::bIsExactType(pNode))   {   ProjectOp::vDescribe(pNode, sBuf, pRootNode); }
  else if ( DataSource::bIsOfType(pNode))      {  DataSource::vDescribe(pNode, sBuf, pRootNode); }
  else if (   FilterOp::bIsExactType(pNode))   {    FilterOp::vDescribe(pNode, sBuf, pRootNode); }
  else if (    LimitOp::bIsExactType(pNode))   {     LimitOp::vDescribe(pNode, sBuf, pRootNode); }
  else if (    UnionOp::bIsExactType(pNode))   {     UnionOp::vDescribe(pNode, sBuf, pRootNode); }
  else if (    OrderOp::bIsExactType(pNode))   {     OrderOp::vDescribe(pNode, sBuf, pRootNode); }
  else if (  ExplainOp::bIsExactType(pNode))   {   ExplainOp::vDescribe(pNode, sBuf, pRootNode); }
  else if (     JoinOp::bIsOfType(pNode))      {      JoinOp::vDescribe(pNode, sBuf, pRootNode); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SqlOperator::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism here
  if      (AggregateOp::bIsExactType(pNode)) { AggregateOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if (  ProjectOp::bIsExactType(pNode)) {   ProjectOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if ( DataSource::bIsOfType(pNode))    {  DataSource::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if (   FilterOp::bIsExactType(pNode)) {    FilterOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if (    LimitOp::bIsExactType(pNode)) {     LimitOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if (    UnionOp::bIsExactType(pNode)) {     UnionOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if (    OrderOp::bIsExactType(pNode)) {     OrderOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if (  ExplainOp::bIsExactType(pNode)) {   ExplainOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else if (     JoinOp::bIsOfType(pNode))    {      JoinOp::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void SqlOperator::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s2K sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object
    return;
  }

  // Manual polymorphism here
  if      (AggregateOp::bIsExactType(szType)) { AggregateOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  else if (  ProjectOp::bIsExactType(szType)) {   ProjectOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  else if ( DataSource::bIsOfType(szType))    {  DataSource::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  else if (   FilterOp::bIsExactType(szType)) {    FilterOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  else if (    LimitOp::bIsExactType(szType)) {     LimitOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  else if (      SetOp::bIsOfType(szType))    {       SetOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  else if (    OrderOp::bIsExactType(szType)) {     OrderOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  else if (  ExplainOp::bIsExactType(szType)) {   ExplainOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
  else if (     JoinOp::bIsOfType(szType))    {      JoinOp::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }
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
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 SqlOperator::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_SQLOPERATOR_DEFN);
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlOperator::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SQLOPERATOR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SqlOperator::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_SQLOPERATOR_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions      */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* SqlOperator::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
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
  else if (    OrderOp::bIsOfType(pNode)) { return     OrderOp::pColumnExprs(pNode, pRootNode); }
  else if (  ExplainOp::bIsOfType(pNode)) { return   ExplainOp::pColumnExprs(pNode, pRootNode); }
  else if (     JoinOp::bIsOfType(pNode)) { return      JoinOp::pColumnExprs(pNode, pRootNode); }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

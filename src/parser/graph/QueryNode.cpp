/*============================================================================*/
/* QueryNode.cpp - An mutable node that holds the plan of the user's query    */
/*============================================================================*/

#include "parser/stages/parser/Parser.h"
#include "QueryNode.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,QueryNode,CN_QUERYNODE)
__aNewFunction(QueryNode,CN_QUERYNODE)
__vDeleteFunction(QueryNode,CN_QUERYNODE)
__aFooterFunction(QueryNode)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
QueryNode::QueryNode(Stz* szName, Stz* szType, Value xValue,
                               PlanNodeList* pProperties, PlanNodeList* pChildren)
    : MutablePlanNode(CN_QUERYNODE, szName, szType, xValue, pProperties, pChildren)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

  _checkInitialised_(CN_QUERYNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
QueryNode::~QueryNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


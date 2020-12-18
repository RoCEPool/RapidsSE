/*============================================================================*/
/* StatisticsNode.cpp - An mutable node that holds stats for the user's query */
/*============================================================================*/

#include "parser/Parser.h"
#include "StatisticsNode.hpp"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(Parser,StatisticsNode,CN_STATISTICSNODE)
__aNewFunction(StatisticsNode,CN_STATISTICSNODE)
__vDeleteFunction(StatisticsNode,CN_STATISTICSNODE)
__aFooterFunction(StatisticsNode)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StatisticsNode::StatisticsNode(Stz* szName, Stz* szType, Value xValue,
                               PlanNodeList* pProperties, PlanNodeList* pChildren)
    : MutablePlanNode(CN_STATISTICSNODE, szName, szType, xValue, pProperties, pChildren)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATISTICSNODE,_FOOTER_);

  _checkInitialised_(CN_STATISTICSNODE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STATISTICSNODE,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StatisticsNode::~StatisticsNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATISTICSNODE,_FOOTER_);

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


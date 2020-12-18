/*============================================================================*/
/* Parser.h -- Parser namespace                                              */
/*============================================================================*/
#pragma once

#include "Plans.h"

//------------------------------------------------------------------------------

namespace Parser
{  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "ExprAtom.hpp"
#include "ExprColumnRef.hpp"
#include "ExprList.hpp"
#include "SqlPlanner.hpp"
#include "TableRef.hpp"

#include "graph/PlanNodeList.hpp"
#include "graph/PlanNode.hpp"
#include "graph/PlanNodeBuilderList.hpp"
#include "graph/PlanNodeBuilder.hpp"
#include "graph/DefinitionNode.hpp"
#include "graph/PrimitiveNode.hpp"
#include "graph/MutablePlanNode.hpp"
#include "graph/QueryNode.hpp"
#include "graph/MetadataNode.hpp"
#include "graph/StatisticsNode.hpp"
#include "graph/PlanDefinitionParser.hpp"
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

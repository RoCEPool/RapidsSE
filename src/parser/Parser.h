/*============================================================================*/
/* Parser.h -- Parser namespace                                              */
/*============================================================================*/
#pragma once

#include "DynamoCompiler.h"

//------------------------------------------------------------------------------

namespace Parser
{  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "parser/stages/parser/ExprAtom.hpp"
#include "parser/stages/parser/ExprColumnRef.hpp"
#include "parser/stages/parser/ExprList.hpp"
#include "parser/stages/parser/SqlPlanner.hpp"
#include "parser/stages/parser/TableRef.hpp"

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

#include "graphTools/PlanWalker.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

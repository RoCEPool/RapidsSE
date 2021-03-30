/*============================================================================*/
/* SqlOperator.hpp - an abstract base class of all objects that process       */
/* streams of row data.                                                       */
/*============================================================================*/
#pragma once

#include "PlanObject.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class PlanNode;

  class SqlOperator : public PlanObject
  {

  private:
    // Shalt never be instantiated. Static functions only.
    SqlOperator();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Return a list of ProjectedExprs that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static PlanNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

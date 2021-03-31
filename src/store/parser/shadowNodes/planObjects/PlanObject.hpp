/*============================================================================*/
/* PlanObject.hpp - an abstract base class of all object that appear in the   */
/* main data path of the plan graph.                                          */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  class PlanObject : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    PlanObject();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

  protected:
    // Indent format
    enum eIndentFormat {
      IF_NONE = 0,
      IF_PLAIN,
      IF_ARROW,
      IF_NENUMS
    };

    static void vAppendIndent(s48K& sBuf, i8 iIndentLevel, eIndentFormat eFormat);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

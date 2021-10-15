/*============================================================================*/
/* IndexDefn.hpp - a definition of a index used in the plan                   */
/* Note: There is already an IndexDef.hpp file in the project. Needed to      */
/* give this file a unique name.                                              */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class IndexDefn : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    IndexDefn();

  public:
    static QueryNode* pCreate(Stz* szIndexName, QueryNode* pColumnRefs, b1 bNotNull);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static Stz*           szIndexName(QueryNode* pNode);
    static i8             iNumColumns(QueryNode* pNode);
    static PlanNodeIter*  pColumnIter(QueryNode* pNode);
    static b1             bIsUnique(QueryNode* pNode);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

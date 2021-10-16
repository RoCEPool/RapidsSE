/*============================================================================*/
/* UnionOp.hpp - a union of two SELECT queries                                */
/*============================================================================*/
#pragma once

#include "SetOp.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class UnionOp : public SetOp
  {
  private:
    // Shalt never be instantiated. Static functions only.
    UnionOp();

  public:
    static QueryNode* pCreate(b1 bIsDistinct, QueryNode* pCorrelatedByList);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static void       vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    // Returns the SQL keyword that this operator is known by. e.g UNION
    static Stz* szSqlKeyword(QueryNode* pNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

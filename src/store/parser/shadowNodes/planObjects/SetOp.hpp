/*============================================================================*/
/* SetOp.hpp - base class of all set operators                                */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  class SetOp : public SqlOperator
  {
  private:
    // Shalt never be instantiated. Static functions only.
    SetOp();

  protected:
    // Shouldn't be able to call this directly unless you are a subclass
    static QueryNode* pCreate(b1 bIsDistinct, QueryNode* pCorrelatedByList);

  public:
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

    // Returns true if non-unique rows are to be filtered out
    static b1 bIsDistinct(QueryNode* pNode);

    // Returns the node representing the list of columns that are to used.
    // This node may be null or empty.
    static QueryNode* pCorrespondingByList(QueryNode* pNode);

    // Remove and null the CORRESPONDING BY list
    static void vRemoveCorrespondingByList(QueryNode* pNode);

    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

    // Returns the SQL keyword that this operator is known by. e.g UNION
    static Stz* szSqlKeyword(QueryNode* pNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

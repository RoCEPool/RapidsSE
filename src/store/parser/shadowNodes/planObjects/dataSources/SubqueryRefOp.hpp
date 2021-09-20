/*============================================================================*/
/* SubqueryRefOp.hpp - references a table subquery elsewhere in the plan      */
/*============================================================================*/
#pragma once

#include "DataSource.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  class SubqueryRefOp : public DataSource
  {
  private:
    // Shalt never be instantiated. Static functions only.
    SubqueryRefOp();

  public:
    static QueryNode* pCreate(Stz* pSubqueryAlias);
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    // Returns the name of the virtual table that the table subquery is known as
    static Stz* szAliasName(PlanNode* pNode);

    // TODO: add a method to look up and return the QueryNode* of the referenced subquery
    //       static QueryNode* pSubqueryNode(QueryNode* pNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

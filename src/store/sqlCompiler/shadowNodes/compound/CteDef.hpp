/*============================================================================*/
/* CteDef.hpp - a definition of a Common Table Expression                     */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class CteDef : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    CteDef();

  public:
    static QueryNode* pCreate(Stz* szAliasName);
    static QueryNode* pCreate(Stz* szAliasName, QueryNode* pColumnDefs);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static Stz* szAliasName(QueryNode* pNode);

    static QueryNode* pColumnDef(QueryNode* pNode, Stz* szColumnName);
    static PlanNodeIter* pColumnDefIter(QueryNode* pNode);
    static i8 nColumns(QueryNode* pNode);
    static void vAddColumnDef(QueryNode* pNode, QueryNode* pColumnDef);

    // Return the first column definition
    static QueryNode* pFirstColumnDef(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

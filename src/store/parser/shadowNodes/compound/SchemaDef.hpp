/*============================================================================*/
/* SchemaDef.hpp - a definition of a schema used in the plan                  */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class SchemaDef : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    SchemaDef();

  public:
    static QueryNode* pCreate(Stz* szCatalogName, Stz* szSchemaName, QueryNode* pTableDefs);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static Stz* szCatalogName(QueryNode* pNode);
    static Stz* szSchemaName(QueryNode* pNode);

    static QueryNode* pTableDef(QueryNode* pNode, Stz* szTableName);
    static PlanNodeIter* pTableDefIter(QueryNode* pNode);

    static void vAddTable(QueryNode* pNode, QueryNode* pTableDefNode);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

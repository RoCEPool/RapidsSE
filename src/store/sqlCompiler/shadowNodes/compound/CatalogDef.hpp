/*============================================================================*/
/* CatalogDef.hpp - a definition of a catalogs used in the plan               */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class CatalogDef : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    CatalogDef();

  public:
    static QueryNode* pCreate(Stz* szCatalogName, QueryNode* pSchemaDefs);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static Stz* szCatalogName(QueryNode* pNode);

    static QueryNode* pSchemaDef(QueryNode* pNode, Stz* szSchemaName);
    static PlanNodeIter* pSchemaDefIter(QueryNode* pNode);

    static void vAddSchema(QueryNode* pNode, QueryNode* pSchemaDefNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

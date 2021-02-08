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
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    static Stz* szCatalogName(QueryNode* pNode);
    static Stz* szSchemaName(QueryNode* pNode);
    static PlanNodeIter* pTableDefIter(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* CatalogDefs.hpp - a list of catalog definitions                            */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class CatalogDefs : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    CatalogDefs();

  public:
    static QueryNode* pCreate();
    static void       vAddCatalogDef(QueryNode* pNode, QueryNode* pCatalogDef);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    static i8 nCatalogs(QueryNode* pCatalogDefs);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

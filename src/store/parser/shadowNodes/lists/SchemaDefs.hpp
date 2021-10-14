/*============================================================================*/
/* SchemaDefs.hpp - a list of schema definitions                              */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class SchemaDefs : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    SchemaDefs();

  public:
    static QueryNode* pCreate();
    static void       vAddSchemaDef(QueryNode* pNode, QueryNode* pSchemaDef);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static i8 nSchemas(QueryNode* pSchemaDefs);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

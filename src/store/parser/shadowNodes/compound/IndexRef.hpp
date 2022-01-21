/*============================================================================*/
/* IndexRef.hpp - a reference to a named index                                */
/*============================================================================*/
#pragma once

#include "TableRef.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class IndexRef : public TableRef
  {

  private:
    // Shalt never be instantiated. Static functions only.
    IndexRef();

  public:
    static QueryNode* pCreate(Text* pIndexName);
    static QueryNode* pCreate(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName, Stz* szIndexName);
    static QueryNode* pCreate(Text* pCatalogName, Text* pSchemaName, Text* pTableName, Text* pIndexName);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static Stz*  szIndex(QueryNode* pIndexRef);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

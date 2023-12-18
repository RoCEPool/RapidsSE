/*============================================================================*/
/* TableRef.hpp - a reference to a named table                                */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class TableRef : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    TableRef();

  protected:
    static QueryNode* pCreate(Stz* szName, Stz* szType, Text* pCatalogName, Text* pSchemaName, Text* pTableName);

  public:
    static QueryNode* pCreate(Text* pCatalogName, Text* pSchemaName, Text* pTableName);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static Stz*  szCatalog(QueryNode* pTableRef);
    static Stz*  szSchema(QueryNode* pTableRef);
    static Stz*  szTable(QueryNode* pTableRef);
    static b1    bIsFullyQualified(QueryNode* pTableRef);
    static void  vQualify(QueryNode* pTableRef, Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

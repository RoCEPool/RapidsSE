/*============================================================================*/
/* Metadata.hpp - a ShadowNode for the /__root__/metadata node and subtree    */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  class MetadataNode : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    MetadataNode();

  public:
    static QueryNode* pCreate();

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns true if metadata has previously been added
    static b1 bHasMetadata(PlanNode* pNode);

    static void vAddCatalog(QueryNode* pMetadataNode, QueryNode* pCatalogDef);
    static void vAddSchema(QueryNode* pMetadataNode, QueryNode* pSchemaDef);
    static void vAddTable(QueryNode* pMetadataNode, QueryNode* pTableDef);

    static QueryNode* pCatalogDef(QueryNode* pMetadataNode, Stz* szCatalogName);
    static QueryNode* pSchemaDef(QueryNode* pMetadataNode, Stz* szCatalogName, Stz* szSchemaName);
    static QueryNode* pTableDef(QueryNode* pMetadataNode, Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName);
    static QueryNode* pColumnDef(QueryNode* pMetadataNode, Stz* szCatalogName,
                                 Stz* szSchemaName, Stz* szTableName, Stz* szColumnName);

    // Get an iterator of PlanNodes that represent CatalogDefs, SchemaDefs, TableDefs, ColumnDefs and IndexDefs
    static PlanNodeIter* pCatalogDefIter(QueryNode* pMetadataNode);
    static PlanNodeIter* pSchemaDefIter(QueryNode* pMetadataNode, Stz* szCatalogName);
    static PlanNodeIter* pTableDefIter(QueryNode* pMetadataNode, Stz* szCatalogName, Stz* szSchemaName);
    static PlanNodeIter* pColumnDefIter(QueryNode* pMetadataNode, Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName);
    static PlanNodeIter* pIndexDefIter(QueryNode* pMetadataNode, Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName);
    static PlanNodeIter* pCteDefIter(QueryNode* pMetadataNode);

    // Returns the node that represents the hierarchy of CteDefs
    static QueryNode* pCteDefsNode(QueryNode* pMetadataNode);

    // Lookup and return a list of nodes that represent TableDefs that match
    // against a given ColumnRef. Returned list needs to be deallocated.
    static QueryNodeList* pMatchColumnRef(QueryNode* pMetadataNode, QueryNode* pColumnRef);

    // Adds a top-level CTE definition to the metadata
    static void vAddCte(QueryNode* pMetadataNode, QueryNode* pCteDef);

    // Looks for a particular CteDef node under this metadata path.
    // Writes the path to sBuf and returns true if found, otherwise false.
    static b1 bGetCteDefPath(QueryNode* pMetadataNode, QueryNode* pCteDef, s2K& sBuf);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

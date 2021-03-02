/*============================================================================*/
/* Metadata.hpp - a ShadowNode for the /__root__/metadata node and subtree    */
/*============================================================================*/

#include "BASE.h"
#include "MetadataNode.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"

#include "CatalogDef.hpp"
#include "SchemaDefs.hpp"
#include "SchemaDef.hpp"
#include "TableDefs.hpp"
#include "TableDef.hpp"
#include "ColumnDef.hpp"
#include "IndexDef.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_METADATANODE_NAME = "metadata"_Stz;
static Stz* C_METADATANODE_DEFN = "/__root__/definitions/object/metadataNode"_Stz;

static Stz* C_CATALOGS_PROP = "catalogs"_Stz;
static Stz* C_CATALOGS_DEFN = "/__root__/definitions/object/list/catalogDefs"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the metadata definition     */
/*----------------------------------------------------------------------------*/
PlanNode* MetadataNode::pCreate()
{
  QueryNode* pMetadataNode = new QueryNode(C_METADATANODE_NAME, C_METADATANODE_DEFN);
  QueryNode* pCatalogsNode = new QueryNode(C_CATALOGS_PROP, C_CATALOGS_DEFN);

  pMetadataNode->vAppendProperty(pCatalogsNode);

  return pMetadataNode;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the metadata definition     */
/*----------------------------------------------------------------------------*/
b1 MetadataNode::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_METADATANODE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Add a catalog definition to the metadata                                   */
/*----------------------------------------------------------------------------*/
void MetadataNode::vAddCatalog(QueryNode* pMetadataNode, QueryNode* pCatalogDef)
{
  ASSERT(bIsOfType(pMetadataNode));
  ASSERT(pCatalogDef);
  ASSERT(CatalogDef::bIsOfType(pCatalogDef));

  // Get the catalogs property
  QueryNode* pCatalogsNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_CATALOGS_PROP, pMetadataNode));
  ASSERT(pCatalogsNode);

  pCatalogsNode->vAppendChild(pCatalogDef);
}

/*----------------------------------------------------------------------------*/
/* Add a schema definition to the metadata                                    */
/*----------------------------------------------------------------------------*/
void MetadataNode::vAddSchema(QueryNode* pMetadataNode, QueryNode* pSchemaDef)
{
  ASSERT(bIsOfType(pMetadataNode));
  ASSERT(pSchemaDef);
  ASSERT(SchemaDef::bIsOfType(pSchemaDef));

  // Get the catalog definition
  Stz* szCatalogName = SchemaDef::szCatalogName(pSchemaDef);
  ASSERT(szCatalogName);

  QueryNode* pCatalogDefNode = pCatalogDef(pMetadataNode, szCatalogName);

  if (!pCatalogDefNode) {
    QueryNode* pSchemaDefs = SchemaDefs::pCreate();
    pCatalogDefNode = CatalogDef::pCreate(szCatalogName, pSchemaDefs);
    vAddCatalog(pMetadataNode, pCatalogDefNode);
  }

  CatalogDef::vAddSchema(pCatalogDefNode, pSchemaDef);
}

/*----------------------------------------------------------------------------*/
/* Add a table definition to the metadata. Creates the schema/catalog if req  */
/*----------------------------------------------------------------------------*/
void MetadataNode::vAddTable(QueryNode* pMetadataNode, QueryNode* pTableDef)
{
  ASSERT(bIsOfType(pMetadataNode));
  ASSERT(pTableDef);
  ASSERT(TableDef::bIsOfType(pTableDef));

  // Get the catalog definition
  Stz* szCatalogName = TableDef::szCatalogName(pTableDef);
  Stz* szSchemaName  = TableDef::szSchemaName(pTableDef);
  ASSERT(szCatalogName);
  ASSERT(szSchemaName);

  QueryNode* pSchemaDefNode = pSchemaDef(pMetadataNode, szCatalogName, szSchemaName);

  if (!pSchemaDefNode) {
    QueryNode* pTableDefs = TableDefs::pCreate();
    pSchemaDefNode = SchemaDef::pCreate(szCatalogName, szSchemaName, pTableDefs);
    vAddSchema(pMetadataNode, pSchemaDefNode);
  }

  SchemaDef::vAddTable(pSchemaDefNode, pTableDef);
}

/*----------------------------------------------------------------------------*/
/* Get a plan node that represents a catalog definition and matches a given   */
/* identifier from the metadata. Will return null if not found.               */
/*----------------------------------------------------------------------------*/
QueryNode* MetadataNode::pCatalogDef(QueryNode* pMetadataNode, Stz* szCatalogName)
{
  ASSERT(bIsOfType(pMetadataNode));
  ASSERT(szCatalogName);

  QueryNode* pCatalogDef = nullptr;

  // Get the catalogs property
  QueryNode* pCatalogsNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_CATALOGS_PROP, pMetadataNode));

  if (!pCatalogsNode) {
    return pCatalogDef;
  }

  PlanNodeIter* pIter = pCatalogsNode->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pChild = static_cast<QueryNode*>(pIter->pNext());
    ASSERT(CatalogDef::bIsOfType(pChild));

    if (CatalogDef::szCatalogName(pChild)->bEqual(szCatalogName)) {
      pCatalogDef = pChild;
      break;
    }
  }

  delete pIter;
  return pCatalogDef;
}

/*----------------------------------------------------------------------------*/
/* Get a plan node that represents a schema definition and matches a given    */
/* identifier from the metadata. Will return null if not found.               */
/*----------------------------------------------------------------------------*/
QueryNode* MetadataNode::pSchemaDef(QueryNode* pMetadataNode, Stz* szCatalogName, Stz* szSchemaName)
{
  ASSERT(bIsOfType(pMetadataNode));
  ASSERT(szCatalogName);
  ASSERT(szSchemaName);

  // Get the catalogDef
  QueryNode* pCatalogDefNode = MetadataNode::pCatalogDef(pMetadataNode, szCatalogName);

  if (pCatalogDefNode == nullptr) {
    return nullptr;
  }

  return CatalogDef::pSchemaDef(pCatalogDefNode, szSchemaName);
}

/*----------------------------------------------------------------------------*/
/* Get a plan node that represents a table definition and matches a given     */
/* identifier from the metadata. Will return null if not found.               */
/*----------------------------------------------------------------------------*/
QueryNode* MetadataNode::pTableDef(QueryNode* pMetadataNode, Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName)
{
  ASSERT(bIsOfType(pMetadataNode));
  ASSERT(szCatalogName);
  ASSERT(szSchemaName);
  ASSERT(szTableName);

  // Get the schemaDef
  QueryNode* pSchemaDefNode = MetadataNode::pSchemaDef(pMetadataNode, szCatalogName, szSchemaName);

  if (pSchemaDefNode == nullptr) {
    return nullptr;
  }

  return SchemaDef::pTableDef(pSchemaDefNode, szTableName);
}

/*----------------------------------------------------------------------------*/
/* Get an iterator of PlanNodes that each represent a CatalogDef.             */
/* Iterator must be freed when finished.                                      */
/*----------------------------------------------------------------------------*/
PlanNodeIter* MetadataNode::pCatalogDefIter(QueryNode* pMetadataNode)
{
  ASSERT(bIsOfType(pMetadataNode));

  // Get the catalogs property
  QueryNode* pCatalogsNode = static_cast<QueryNode*>(GraphNav::pFindProperty(C_CATALOGS_PROP, pMetadataNode));
  ASSERT(pCatalogsNode);

  return pCatalogsNode->pNewChildrenIter();
}

/*----------------------------------------------------------------------------*/
/* Get an iterator of PlanNodes that each represent a SchemaDef for a given   */
/* catalog. May be null if catalog does not exist.                            */
/* Iterator must be freed when finished.                                      */
/*----------------------------------------------------------------------------*/
PlanNodeIter* MetadataNode::pSchemaDefIter(QueryNode* pMetadataNode, Stz* szCatalogName)
{
  QueryNode* pCatalogDefNode = pCatalogDef(pMetadataNode, szCatalogName);

  if (!pCatalogDefNode) {
    return nullptr;
  }

  ASSERT(CatalogDef::bIsOfType(pCatalogDefNode));
  return CatalogDef::pSchemaDefIter(pCatalogDefNode);
}

/*----------------------------------------------------------------------------*/
/* Get an iterator of PlanNodes that each represent a TableDef for a given    */
/* catalog and schema. May be null if catalog or schema does not exist.       */
/* Iterator must be freed when finished.                                      */
/*----------------------------------------------------------------------------*/
PlanNodeIter* MetadataNode::pTableDefIter(QueryNode* pMetadataNode, Stz* szCatalogName, Stz* szSchemaName)
{
  QueryNode* pSchemaDefNode = pSchemaDef(pMetadataNode, szCatalogName, szSchemaName);

  if (!pSchemaDefNode) {
    return nullptr;
  }

  ASSERT(SchemaDef::bIsOfType(pSchemaDefNode));
  return SchemaDef::pTableDefIter(pSchemaDefNode);
}

/*----------------------------------------------------------------------------*/
/* Get an iterator of PlanNodes that each represent a ColumnDef for a given   */
/* catalog, schema and table. May be null if catalog or schema does not exist.*/
/* Iterator must be freed when finished.                                      */
/*----------------------------------------------------------------------------*/
PlanNodeIter* MetadataNode::pColumnDefIter(QueryNode* pMetadataNode, Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName)
{
  QueryNode* pTableDefNode = pTableDef(pMetadataNode, szCatalogName, szSchemaName, szTableName);

  if (!pTableDefNode) {
    return nullptr;
  }

  ASSERT(TableDef::bIsOfType(pTableDefNode));
  return TableDef::pColumnDefIter(pTableDefNode);
}

/*----------------------------------------------------------------------------*/
/* Get an iterator of PlanNodes that each represent a IndexDef for a given    */
/* catalog, schema and table. May be null if catalog or schema does not exist.*/
/* Iterator must be freed when finished.                                      */
/*----------------------------------------------------------------------------*/
PlanNodeIter* MetadataNode::pIndexDefIter(QueryNode* pMetadataNode, Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName)
{
  QueryNode* pTableDefNode = pTableDef(pMetadataNode, szCatalogName, szSchemaName, szTableName);

  if (!pTableDefNode) {
    return nullptr;
  }

  ASSERT(TableDef::bIsOfType(pTableDefNode));
  return TableDef::pIndexDefIter(pTableDefNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

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

#include "ColumnRef.hpp"

#include "CteDef.hpp"
#include "CteDefs.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_METADATANODE_NAME = "metadata"_Stz;
static Stz* C_METADATANODE_DEFN = "/__root__/definitions/object/metadataNode"_Stz;

static Stz* C_CATALOGS_PROP = "catalogs"_Stz;
static Stz* C_CATALOGS_DEFN = "/__root__/definitions/object/list/catalogDefs"_Stz;

static Stz* C_CTES_PROP = "ctes"_Stz;
static Stz* C_CTES_DEFN = "/__root__/definitions/object/list/cteDefs"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the metadata definition     */
/*----------------------------------------------------------------------------*/
QueryNode* MetadataNode::pCreate()
{
  QueryNode* pMetadataNode = new QueryNode(C_METADATANODE_NAME, C_METADATANODE_DEFN);
  QueryNode* pCatalogsNode = new QueryNode(C_CATALOGS_PROP, C_CATALOGS_DEFN);
  QueryNode* pCtesNode     = new QueryNode(C_CTES_PROP, C_CTES_DEFN);

  pMetadataNode->vAppendProperty(pCatalogsNode);
  pMetadataNode->vAppendProperty(pCtesNode);

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
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 MetadataNode::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_METADATANODE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if metadata has previously been added                         */
/*----------------------------------------------------------------------------*/
b1 MetadataNode::bHasMetadata(PlanNode *pNode)
{
  ASSERT(pNode);
  ASSERT(bIsOfType(pNode));

  // Get the catalogs property
  QueryNode* pCatalogsNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOGS_PROP, pNode));
  ASSERT(pCatalogsNode);

  return pCatalogsNode->nChildren() > 0;
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
  QueryNode* pCatalogsNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOGS_PROP, pMetadataNode));
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
  QueryNode* pCatalogsNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOGS_PROP, pMetadataNode));

  if (!pCatalogsNode) {
    return pCatalogDef;
  }

  PlanNodeIter* pIter = pCatalogsNode->pNewChildrenIter();

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(CatalogDef::bIsOfType(pChild));

    if (CatalogDef::szCatalogName(pChild)->bSame(szCatalogName)) {
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
/* Get a plan node that represents a column definition and matches a given    */
/* identifier from the metadata. Will return null if not found.               */
/*----------------------------------------------------------------------------*/
QueryNode* MetadataNode::pColumnDef(QueryNode* pMetadataNode, Stz* szCatalogName,
                                    Stz* szSchemaName, Stz* szTableName, Stz* szColumnName)
{
  ASSERT(bIsOfType(pMetadataNode));
  ASSERT(szCatalogName);
  ASSERT(szSchemaName);
  ASSERT(szTableName);
  ASSERT(szColumnName);

  // Get the tableDef
  QueryNode* pTableDefNode = MetadataNode::pTableDef(pMetadataNode, szCatalogName, szSchemaName, szTableName);

  if (pTableDefNode == nullptr) {
    return nullptr;
  }

  return TableDef::pColumnDef(pTableDefNode, szColumnName);
}

/*----------------------------------------------------------------------------*/
/* Get a plan node that represents an index definition and matches a given    */
/* identifier from the metadata. Will return null if not found.               */
/*----------------------------------------------------------------------------*/
QueryNode* MetadataNode::pIndexDef(QueryNode* pMetadataNode, Stz* szCatalogName,
                                    Stz* szSchemaName, Stz* szTableName, Stz* szIndexName)
{
  ASSERT(bIsOfType(pMetadataNode));
  ASSERT(szCatalogName);
  ASSERT(szSchemaName);
  ASSERT(szTableName);
  ASSERT(szIndexName);

  // Get the tableDef
  QueryNode* pTableDefNode = MetadataNode::pTableDef(pMetadataNode, szCatalogName, szSchemaName, szTableName);

  if (pTableDefNode == nullptr) {
    return nullptr;
  }

  return TableDef::pIndexDef(pTableDefNode, szIndexName);
}

/*----------------------------------------------------------------------------*/
/* Get an iterator of PlanNodes that each represent a CatalogDef.             */
/* Iterator must be freed when finished.                                      */
/*----------------------------------------------------------------------------*/
PlanNodeIter* MetadataNode::pCatalogDefIter(QueryNode* pMetadataNode)
{
  ASSERT(bIsOfType(pMetadataNode));

  // Get the catalogs property
  QueryNode* pCatalogsNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOGS_PROP, pMetadataNode));
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

/*----------------------------------------------------------------------------*/
/* Get an iterator of PlanNodes that each represent a CteDef.                 */
/* Only iterates the immediate children, no nested CteDefs.                   */
/* Iterator must be freed when finished.                                      */
/*----------------------------------------------------------------------------*/
PlanNodeIter* MetadataNode::pCteDefIter(QueryNode* pMetadataNode)
{
  ASSERT(bIsOfType(pMetadataNode));

  // Get the ctes property
  QueryNode* pCtesNode = _pQueryNode(GraphNav::pFindProperty(C_CTES_PROP, pMetadataNode));
  ASSERT(pCtesNode);
  ASSERT(CteDefs::bIsOfType(pCtesNode));

  return pCtesNode->pNewChildrenIter();
}

/*----------------------------------------------------------------------------*/
/* Returns the node that represents the hierarchy of CteDefs                  */
/*----------------------------------------------------------------------------*/
QueryNode* MetadataNode::pCteDefsNode(QueryNode* pMetadataNode)
{
  ASSERT(bIsOfType(pMetadataNode));

  // Get the ctes property
  QueryNode* pCtesNode = _pQueryNode(GraphNav::pFindProperty(C_CTES_PROP, pMetadataNode));
  ASSERT(pCtesNode);
  ASSERT(CteDefs::bIsOfType(pCtesNode));

  return pCtesNode;
}

/*----------------------------------------------------------------------------*/
/* Return a list of TableDef nodes in the metadata that match a columnRef     */
/*----------------------------------------------------------------------------*/
QueryNodeList* MetadataNode::pMatchColumnRef(QueryNode* pMetadataNode, QueryNode* pColumnRef)
{
  ASSERT(pMetadataNode);
  ASSERT(pColumnRef);

  QueryNodeList* pTableDefList = new QueryNodeList();
  
  Stz* szCatalogName = ColumnRef::szCatalog(pColumnRef);
  Stz* szSchemaName  = ColumnRef::szSchema(pColumnRef);
  Stz* szTableName   = ColumnRef::szTable(pColumnRef);
  Stz* szColumnName  = ColumnRef::szColumn(pColumnRef);
  
  ASSERT(szColumnName);
  
  // First get the catalogs property
  QueryNode* pCatalogsNode = _pQueryNode(GraphNav::pFindProperty(C_CATALOGS_PROP, pMetadataNode));
  
  // Iterate every catalog
  PlanNodeIter* pCatalogIter = pCatalogsNode->pNewChildrenIter();
  
  while (pCatalogIter->bHasNext()) {
    QueryNode* pCatalogDef = _pQueryNode(pCatalogIter->pNext());
    
    if (szCatalogName && !szCatalogName->bSame(CatalogDef::szCatalogName(pCatalogDef))) {
      continue;
    }
    
    // Iterate the schemas of this catalogDef
    PlanNodeIter* pSchemaIter = CatalogDef::pSchemaDefIter(pCatalogDef);
    
    while (pSchemaIter->bHasNext()) {
      QueryNode* pSchemaDef = _pQueryNode(pSchemaIter->pNext());

      if (szSchemaName && !szSchemaName->bSame(SchemaDef::szSchemaName(pSchemaDef))) {
        continue;
      }
      
      // Iterate the tables of this schemaDef
      PlanNodeIter* pTableIter = SchemaDef::pTableDefIter(pSchemaDef);
      
      while (pTableIter->bHasNext()) {
        QueryNode* pTableDef = _pQueryNode(pTableIter->pNext());

        if (szTableName && !szTableName->bSame(TableDef::szTableName(pTableDef))) {
          continue;
        }
        
        // Iterate the columns of this tableDef
        PlanNodeIter* pColumnIter = TableDef::pColumnDefIter(pTableDef);

        while (pColumnIter->bHasNext()) {
          QueryNode *pColumnDef = static_cast<QueryNode *>(pColumnIter->pNext());

          if (szColumnName && !szColumnName->bSame(ColumnDef::szColumnName(pColumnDef))) {
            continue;
          }

          // We've got a match for the columnRef! Save this table definition
          // but keep looking for other ambiguous matches.
          pTableDefList->vAppend(pTableDef);
        }

        delete pColumnIter;
      }
      
      delete pTableIter;
    }
    
    delete pSchemaIter;
  }  

  delete pCatalogIter;

  return pTableDefList;
}


/*----------------------------------------------------------------------------*/
/* Add a CTE definition to the metadata                                       */
/*----------------------------------------------------------------------------*/
void MetadataNode::vAddCte(QueryNode* pMetadataNode, QueryNode* pCteDef)
{
  ASSERT(bIsOfType(pMetadataNode));
  ASSERT(pCteDef);
  ASSERT(CteDef::bIsOfType(pCteDef));

  // Get the ctes property
  QueryNode* pCtesNode = _pQueryNode(GraphNav::pFindProperty(C_CTES_PROP, pMetadataNode));
  ASSERT(pCtesNode);

  pCtesNode->vAppendChild(pCteDef);
}

/*----------------------------------------------------------------------------*/
/* Looks for a particular CteDef node under this metadata path.               */
/* Returns true if found & sBuf will contain an absolute path.                */
/*----------------------------------------------------------------------------*/
b1 MetadataNode::bGetCteDefPath(QueryNode* pMetadataNode, QueryNode* pCteDef, s2K& sBuf)
{
  ASSERT(pMetadataNode);
  ASSERT(bIsOfType(pMetadataNode));
  ASSERT(pCteDef);
  ASSERT(CteDef::bIsOfType(pCteDef));

  s2K sPathBuf;
  b1 bFound = GraphNav::bGetPath(pCteDef, pMetadataNode, sPathBuf);

  if (bFound) {
    // Relative to absolute path
    sBuf.vCatSprint("%s/%s"_Stz, C_ROOT_PATH, sPathBuf.szStz());
  }

  return bFound;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* SchemaDefs.hpp - a list of schemas definitions                               */
/*============================================================================*/

#include "BASE.h"
#include "SchemaDefs.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "PlanConstants.hpp"
#include "SchemaDef.hpp"

#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_SCHEMADEFS_DEFN = "/__root__/definitions/object/list/schemaDefs"_Stz;
static Stz* C_SCHEMADEFS_NAME = "schemaDefs"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Helper function to create a list of schemaDef nodes.                       */
/*----------------------------------------------------------------------------*/
QueryNode* SchemaDefs::pCreate()
{
  _static_

  // Create an empty schemaDefs object
  QueryNode* pSchemaDefs = new QueryNode(C_SCHEMADEFS_NAME, const_cast<Stz*>(C_SCHEMADEFS_DEFN));

  return pSchemaDefs;
}

/*----------------------------------------------------------------------------*/
/* Add a schemaDef to the list                                                */
/*----------------------------------------------------------------------------*/
void SchemaDefs::vAddSchemaDef(QueryNode* pNode, QueryNode* pSchemaDef)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(SchemaDef::bIsOfType(pSchemaDef));

  pNode->vAppendChild(pSchemaDef);
}

/*----------------------------------------------------------------------------*/
/* Appends a description of the schema definitions into a buffer              */
/*----------------------------------------------------------------------------*/
void SchemaDefs::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_

  ASSERT(bIsOfType(pNode));

  QueryNode* pSchemaDefs = _pQueryNode(pNode);

  PlanNodeIter* pIter = pSchemaDefs->pNewChildrenIter();
  b1 bFirst = true;

  while (pIter->bHasNext()) {
    PlanNode* pChild = pIter->pNext();

    if (bFirst) {
      bFirst = false;
    } else {
      sBuf.vConcat(", "_Stz);
    }

    sBuf.vConcat(SchemaDef::szSchemaName(_pQueryNode(pChild)));
  }

  delete pIter;
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the schemaDefs definition   */
/*----------------------------------------------------------------------------*/
b1 SchemaDefs::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_SCHEMADEFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 SchemaDefs::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_SCHEMADEFS_DEFN);
}

/*----------------------------------------------------------------------------*/
/* get the number of schemas in this list                                     */
/*----------------------------------------------------------------------------*/
i8 SchemaDefs::nSchemas(QueryNode* pSchemaDefs)
{
  ASSERT(bIsOfType(pSchemaDefs));

  return pSchemaDefs->nChildren();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

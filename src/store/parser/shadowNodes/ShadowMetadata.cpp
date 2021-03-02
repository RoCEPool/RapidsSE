/*============================================================================*/
/* ShadowMetadata.hpp - a shadow node of the main /__root__/metadata node     */
/*============================================================================*/

#include "BASE.h"
#include "ShadowMetadata.hpp"

#include "QueryNode.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_METADATA_NAME = "metadata"_Stz;
static Stz* C_METADATA_DEFN = "/__root__/definitions/object/metadataNode"_Stz;



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Appends a description of the ProjectedExpr into a buffer                   */
/*----------------------------------------------------------------------------*/
QueryNode* ShadowMetadata::pCreate()
{
  _static_

  QueryNode* pMetadata = new QueryNode(C_METADATA_NAME, const_cast<Stz*>(C_METADATA_DEFN));
  return pMetadata;
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the root node definition    */
/*----------------------------------------------------------------------------*/
b1 ShadowMetadata::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, const_cast<Stz*>(C_METADATA_DEFN));
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

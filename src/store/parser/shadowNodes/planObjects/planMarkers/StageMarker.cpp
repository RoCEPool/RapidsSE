/*============================================================================*/
/* StageMarker.hpp - indicates to the transliteration when a new pipeline     */
/* stage is required                                                          */
/*============================================================================*/

#include "BASE.h"
#include "StageMarker.hpp"

#include "PlanConstants.hpp"
#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "GraphNav.hpp"
#include "NodeNamer.hpp"
#include "Hashcast.hpp"
#include "Expr.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_STAGEMARKER_DEFN = "/__root__/definitions/object/planObject/planMarker/stageMarker"_Stz;

static Stz* C_LOCATION_PROP = "location"_Stz;
static Stz* C_LOCATION_DEFN = "/__root__/definitions/types/location"_Stz;

static Stz* C_ROUTING_PROP  = "routing"_Stz;
static Stz* C_ROUTING_DEFN = "/__root__/definitions/types/routing"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Helper function to create stageMarker nodes.                               */
/*----------------------------------------------------------------------------*/
QueryNode* StageMarker::pCreate(Location::eLO eLocationType, Routing::eRT eRoutingType)
{
  ASSERT(eLocationType > Location::LO_NONE);
  ASSERT(eLocationType < Location::LO_NENUMS);
  ASSERT(eRoutingType  >= Routing::RT_NONE);
  ASSERT(eRoutingType  < Routing::RT_NENUMS);

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_STAGEMARKER_DEFN, sName);

  // Create the object
  QueryNode* pMarker = new QueryNode(sName, C_STAGEMARKER_DEFN);

  // Create and add the properties
  PlanObject::vSetLocation(pMarker, eLocationType);

  QueryNode* pRoutingProp  = Routing::pCreate(eRoutingType);
  pRoutingProp->vSetName(C_ROUTING_PROP);
  pMarker->vAppendProperty(pRoutingProp);

  return pMarker;
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void StageMarker::vDescribe(PlanNode* pNode, s2K& buf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pRootNode);

  QueryNode* pStmt = _pQueryNode(pNode);

  buf.vConcat(GraphNav::szLeafType(pStmt->szType()));
}


/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain to the buf             */
/*----------------------------------------------------------------------------*/
void StageMarker::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Add the object header and local properties
  vExplainCommon(pNode, pRootNode, pExplainNode, pNode->szType(), sBuf, iIndent);

  // Recurse any children
  if (pNode->bHasChildren()) {
    PlanNodeIter* pIter = pNode->pNewChildrenIter();

    while (pIter->bHasNext()) {
      PlanNode* pChild = pIter->pNext();
      ASSERT(PlanObject::bIsOfType(pChild));

      PlanObject::vExplain(pChild, pRootNode, pExplainNode, sBuf, iIndent + 1);
    }

    delete pIter;
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void StageMarker::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (bIsExactType(szType)) {
    // Explain any common attributes of this object
    // Get the parent type and keep recursing back
    s2K sParentType;
    GraphNav::vParentType(szType, sParentType);
    PlanObject::vExplainCommon(pNode, pRootNode, pExplainNode, sParentType, sBuf, iIndent);

    // Location is explained in the common attributes above
    // Explain any local attributes of this object

    // Get the routing property
    sBuf.vConcat("\n"_Stz);
    vAppendIndent(sBuf, iIndent, IF_PLAIN);
    sBuf.vCatSprint("routing: %s"_Stz, szRouting(pNode));

    // Don't like this code being here...should be encapsulated in Hashcast somehow...
    Routing::eRT eRoutingType = StageMarker::eRouting(pNode);
    if (eRoutingType == Routing::RT_HASHCAST) {
      QueryNode* pHashcastNode = StageMarker::pRouting(pNode);
      ASSERT(Hashcast::bIsOfType(pHashcastNode));

      QueryNode* pSymHashExprList = Hashcast::pHashExprList(pHashcastNode);
      QueryNode* pOrdHashExprList = Hashcast::pOrdinalHashExprList(pHashcastNode, pRootNode);

      ASSERT(pSymHashExprList);
      ASSERT(pOrdHashExprList);
      ASSERT(pSymHashExprList->nChildren() == pOrdHashExprList->nChildren());

      i8 i = 0;
      while (i < pSymHashExprList->nChildren()) {
        QueryNode* pSymExpr = _pQueryNode(pSymHashExprList->pNthChild(i));
        QueryNode* pOrdExpr = _pQueryNode(pOrdHashExprList->pNthChild(i));

        s2K sSymBuf;
        s2K sOrdBuf;

        Expr::vDescribe(pSymExpr, sSymBuf, pRootNode);
        Expr::vDescribe(pOrdExpr, sOrdBuf, pRootNode);

        sBuf.vConcat("\n"_Stz);
        vAppendIndent(sBuf, iIndent, IF_PLAIN);
        sBuf.vCatSprint("hashkey: Expr = %s ; Ordinal Expr = %s"_Stz, sSymBuf.szStz(), sOrdBuf.szStz());

        ++i;
      }

      delete pOrdHashExprList;
    }

    return;
  }

  // Manual polymorphism of subtypes here
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the statement definition    */
/*----------------------------------------------------------------------------*/
b1 StageMarker::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_STAGEMARKER_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 StageMarker::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_STAGEMARKER_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 StageMarker::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_STAGEMARKER_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 StageMarker::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_STAGEMARKER_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the internal Location node                                         */
/*----------------------------------------------------------------------------*/
QueryNode* StageMarker::pLocation(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pLocationNode = _pQueryNode(GraphNav::pFindProperty(C_LOCATION_PROP, pNode));
  ASSERT(pLocationNode);

  return pLocationNode;
}

/*----------------------------------------------------------------------------*/
/* Returns the enum of the location of this StageMarker                       */
/*----------------------------------------------------------------------------*/
Parser::Location::eLO StageMarker::eLocation(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pLocationNode = pLocation(pNode);
  ASSERT(pLocationNode);

  return Location::eLocationType(pLocationNode);
}

/*----------------------------------------------------------------------------*/
/* Returns the string of the name of the location of this StageMarker         */
/*----------------------------------------------------------------------------*/
Stz* StageMarker::szLocation(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pLocationNode = pLocation(pNode);
  ASSERT(pLocationNode);

  return Location::szLocationType(pLocationNode);
}

/*----------------------------------------------------------------------------*/
/* Returns the internal Routing node                                          */
/*----------------------------------------------------------------------------*/
QueryNode* StageMarker::pRouting(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pRoutingNode = _pQueryNode(GraphNav::pFindProperty(C_ROUTING_PROP, pNode));
  ASSERT(pRoutingNode);

  return pRoutingNode;
}

/*----------------------------------------------------------------------------*/
/* Returns the enum of the routing of this StageMarker                        */
/*----------------------------------------------------------------------------*/
Parser::Routing::eRT StageMarker::eRouting(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pRoutingNode = pRouting(pNode);
  ASSERT(pRoutingNode);

  return Routing::eRoutingType(pRoutingNode);
}

/*----------------------------------------------------------------------------*/
/* Returns the string of the name of the routing of this StageMarker          */
/*----------------------------------------------------------------------------*/
Stz* StageMarker::szRouting(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  QueryNode* pRoutingNode = pRouting(pNode);
  ASSERT(pRoutingNode);

  return Routing::szRoutingType(pRoutingNode);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* PlanObject.hpp - an abstract base class of all object that appear in the   */
/* main data path of the plan graph.                                          */
/*============================================================================*/

#include "BASE.h"
#include "PlanObject.hpp"
#include "QueryNodeList.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "store/parser/shadowNodes/planObjects/planMarkers/PlanMarker.hpp"
#include "store/parser/shadowNodes/planObjects/planMarkers/statementMarkers/StatementMarker.hpp"
#include "SqlOperator.hpp"
#include "ExplainOp.hpp"
#include "JoinOp.hpp"
#include "GraphNav.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_PLANOBJECT_DEFN = "/__root__/definitions/object/planObject"_Stz;

static Stz* C_LOCATION_PROP = "location"_Stz;
static Stz* C_LOCATION_DEFN = "/__root__/definitions/types/location"_Stz;


static Stz* aIndentFormat[] = {
    /*IF_NONE*/   "<invalid>"_Stz,
    /*IF_PLAIN*/  "   "_Stz,
    /*IF_ARROW*/  "-> "_Stz,
    /*IF_NENUMS*/ "<invalid>"_Stz
};

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void PlanObject::vDescribe(PlanNode* pNode, s2K& sBuf)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // TODO: manual polymorphism here

  if      ( PlanMarker::bIsOfType(pNode))   {  PlanMarker::vDescribe(pNode, sBuf);    }
  else if (SqlOperator::bIsOfType(pNode))   { SqlOperator::vDescribe(pNode, sBuf); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void PlanObject::vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism here

  if      ( PlanMarker::bIsOfType(pNode))   {  PlanMarker::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent);    }
  else if (SqlOperator::bIsOfType(pNode))   { SqlOperator::vExplain(pNode, pRootNode, pExplainNode, sBuf, iIndent); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void PlanObject::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (PlanObject::bIsExactType(szType)) {
    // Explain any common attributes of this object
    // This is as far as we recurse back

    if (sBuf.nLength() > 0) {
      sBuf.vConcat("\n\n"_Stz);
    }

    // Print the indent before we print the object type/name
    vAppendIndent(sBuf, iIndent, eIndentFormat::IF_ARROW);

    if (pExplainNode && ExplainOp::bWithNames(pExplainNode)) {
      // Print the object name (e.g. FilterOp9123)
      sBuf.vConcat(pNode->szName());

    } else {
      // Print the object type (e.g. FilterOp)
      // Special case for StatementMarkers
      if (StatementMarker::bIsOfType(pNode)) {
        sBuf.vCatSprint("statementMarker: %s"_Stz, GraphNav::szLeafType(pNode->szType()));

      } else {
        sBuf.vConcat(GraphNav::szLeafType(pNode->szType()));
      }
    }


    // Explain any local attributes of this object

    // Explain the optional location property
    QueryNode* pLocation = _pQueryNode(GraphNav::pFindProperty(C_LOCATION_PROP, pNode));

    if (pLocation) {
      Stz* szLocation = Location::szLocationType(pLocation);

      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, eIndentFormat::IF_PLAIN);
      sBuf.vCatSprint("location: %s"_Stz, szLocation);
    }

    return;
  }


  // Manual polymorphism here

  if      ( PlanMarker::bIsOfType(szType))   {  PlanMarker::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent);    }
  else if (SqlOperator::bIsOfType(szType))   { SqlOperator::vExplainCommon(pNode, pRootNode, pExplainNode, szType, sBuf, iIndent); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the objects definition      */
/*----------------------------------------------------------------------------*/
b1 PlanObject::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PLANOBJECT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given type str is derived from the objects definition  */
/*----------------------------------------------------------------------------*/
b1 PlanObject::bIsOfType(Stz* szType)
{
  return ShadowNode::bIsOfType(szType, C_PLANOBJECT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 PlanObject::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_PLANOBJECT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 PlanObject::bIsExactType(Stz* szType)
{
  return ShadowNode::bIsExactType(szType, C_PLANOBJECT_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Return a list of ProjectedColumns that represent the column expressions    */
/* that this PlanObject will return. All nodes and the list must be freed.    */
/*----------------------------------------------------------------------------*/
QueryNodeList* PlanObject::pColumnExprs(QueryNode* pNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism here

  if      ( PlanMarker::bIsOfType(pNode))   { return  PlanMarker::pColumnExprs(pNode, pRootNode);  }
  else if (SqlOperator::bIsOfType(pNode))   { return SqlOperator::pColumnExprs(pNode, pRootNode);  }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Set the location where this planObject is supposed to run in the cluster   */
/*----------------------------------------------------------------------------*/
void PlanObject::vSetLocation(QueryNode *pNode, Location::eLO eLocation)
{
  ASSERT(pNode);
  ASSERT(PlanObject::bIsOfType(pNode));
  ASSERT(eLocation > Location::LO_NONE);
  ASSERT(eLocation < Location::LO_NENUMS);

  vClearLocation(pNode);
  QueryNode* pLocation = Location::pCreate(eLocation);
  pLocation->vSetName(C_LOCATION_PROP);
  pNode->vAppendProperty(pLocation);
}

/*----------------------------------------------------------------------------*/
/* Clear the location where this planObject is supposed to run in the cluster */
/*----------------------------------------------------------------------------*/
void PlanObject::vClearLocation(QueryNode *pNode)
{
  ASSERT(pNode);
  ASSERT(PlanObject::bIsOfType(pNode));

  QueryNode* pLocation = pNode->pRemoveProperty(C_LOCATION_PROP);

  if (pLocation) {
    delete pLocation;
  }
}

/*----------------------------------------------------------------------------*/
/* Returns the location of this node, otherwise if the node doesn't have a    */
/* location then it can descend into the children until a location is found.  */
/* If no location is set then LO_NONE is returned.                            */
/*----------------------------------------------------------------------------*/
Parser::Location::eLO PlanObject::eLocation(QueryNode* pNode, b1 bIncludeChildren)
{
  ASSERT(pNode);
  ASSERT(PlanObject::bIsOfType(pNode));

  Location::eLO eLocType;

  QueryNode* pLocationNode = _pQueryNode(GraphNav::pFindProperty(C_LOCATION_PROP, pNode));

  if (pLocationNode) {
    // There is a location set on this node. Return it.
    ASSERT(Location::bIsOfType(pLocationNode));

    eLocType = Location::eLocationType(pLocationNode);
    ASSERT(eLocType > Location::LO_NONE);
    ASSERT(eLocType < Location::LO_NENUMS);

    return eLocType;
  }

  // No location set on this node.
  if (!bIncludeChildren) {
    return Location::LO_NONE;
  }

  // If it's a JoinOp, then try the right child only.
  // This is because the join runs in the same location
  // as the right side (second) child.
  if (JoinOp::bIsOfType(pNode)) {
    ASSERT(pNode->nChildren() == 2);
    return eLocation(_pQueryNode(pNode->pSecondChild()), bIncludeChildren);
  }

  // Try each child.
  PlanNodeIter* pIter = pNode->pNewChildrenIter();
  eLocType = Location::LO_NONE;

  while (pIter->bHasNext()) {
    QueryNode* pChild = _pQueryNode(pIter->pNext());
    ASSERT(PlanObject::bIsOfType(pChild));

    // Recurse into the child node
    Location::eLO eLoc = eLocation(pChild, bIncludeChildren);

    if (eLoc > Location::LO_NONE && eLoc < Location::LO_NENUMS) {
      eLocType = eLoc;
      break;
    }
  }

  delete pIter;
  return eLocType;
}

/*----------------------------------------------------------------------------*/
/* Returns true if this specific node has a location property set             */
/*----------------------------------------------------------------------------*/
b1 PlanObject::bNodeHasLocation(QueryNode* pNode)
{
  ASSERT(pNode);
  ASSERT(PlanObject::bIsOfType(pNode));

  QueryNode* pLocationNode = _pQueryNode(GraphNav::pFindProperty(C_LOCATION_PROP, pNode));

  if (!pLocationNode) {
    return false;
  }

  Location::eLO eLocType = Location::eLocationType(pLocationNode);
  ASSERT(eLocType > Location::LO_NONE);
  ASSERT(eLocType < Location::LO_NENUMS);
  UNUSED(eLocType); // in release
  return true;
}

/*----------------------------------------------------------------------------*/
/* Appends an indent string of a given format for the given indent level      */
/*----------------------------------------------------------------------------*/
void PlanObject::vAppendIndent(s48K &sBuf, i8 iIndentLevel, eIndentFormat eFormat)
{
  ASSERT(iIndentLevel >= 0);
  ASSERT(eFormat > PlanObject::IF_NONE);
  ASSERT(eFormat < PlanObject::IF_NENUMS);

  Stz* szIndent = aIndentFormat[eFormat];

  if (iIndentLevel == 0) {
    if (eFormat == IF_ARROW) {
      // This just makes it clearer to identify the main subquery and any table subqueries
      sBuf.vConcat("\n∆ "_Stz);
    } else if (eFormat == IF_PLAIN) {
      sBuf.vConcat("  "_Stz);
    }
  }

  for (i8 i=0; i<iIndentLevel; i++) {
    sBuf.vConcat(szIndent);
  }

  if (eFormat == IF_PLAIN) {
    sBuf.vConcat("» "_Stz);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

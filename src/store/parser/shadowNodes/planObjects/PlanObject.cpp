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
#include "SqlOperator.hpp"
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
void PlanObject::vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual polymorphism here

  if      ( PlanMarker::bIsOfType(pNode))   {  PlanMarker::vExplain(pNode, pRootNode, sBuf, iIndent);    }
  else if (SqlOperator::bIsOfType(pNode))   { SqlOperator::vExplain(pNode, pRootNode, sBuf, iIndent); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void PlanObject::vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, Stz* szType, s48K &sBuf, i8 iIndent)
{
  _static_
  ASSERT(bIsOfType(pNode));

  if (PlanObject::bIsExactType(szType)) {
    // Explain any common attributes of this object
    // This is as far as we recurse back
//    s256 sParentType;
//    GraphNav::vParentType(szType, sParentType);
//    PlanObject::vExplainCommon(pNode, pRootNode, sParentType, sBuf, iIndent);

    // Explain any local attributes of this object

    // Explain the optional location property
    QueryNode* pLocation = _pQueryNode(GraphNav::pFindProperty(C_LOCATION_PROP, pNode));

    if (pLocation) {
      Stz* szLocation = Location::szLocationType(pLocation);

      sBuf.vConcat("\n"_Stz);
      vAppendIndent(sBuf, iIndent, eIndentFormat::IF_PLAIN);
      sBuf.vCatSprint("location: \"%s\""_Stz, szLocation);
    }

    return;
  }


  // Manual polymorphism here

  if      ( PlanMarker::bIsOfType(szType))   {  PlanMarker::vExplainCommon(pNode, pRootNode, szType, sBuf, iIndent);    }
  else if (SqlOperator::bIsOfType(szType))   { SqlOperator::vExplainCommon(pNode, pRootNode, szType, sBuf, iIndent); }

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
  QueryNode* pLocation = pNode->pRemoveProperty(C_LOCATION_PROP);

  if (pLocation) {
    delete pLocation;
  }
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

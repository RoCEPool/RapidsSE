/*============================================================================*/
/* PlanObject.hpp - an abstract base class of all object that appear in the   */
/* main data path of the plan graph.                                          */
/*============================================================================*/

#include "BASE.h"
#include "PlanObject.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "Statement.hpp"
#include "SqlOperator.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_PLANOBJECT_DEFN = "/__root__/definitions/object/planObject"_Stz;


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
  ASSERT(bValidate(pNode));

  QueryNode* pExpr = static_cast<QueryNode*>(pNode);

  // TODO: manual polymorphism here

  // Statement
  if      (Statement::bIsOfType(pNode))     { Statement::vDescribe(pNode, sBuf);    }

  // SqlOperator
  else if (SqlOperator::bIsOfType(pNode))   { SqlOperator::vDescribe(pNode, sBuf);  }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Recurses down the query plan and writes the explain of the stmt to the buf */
/*----------------------------------------------------------------------------*/
void PlanObject::vExplain(PlanNode* pNode, s48K& sBuf, i8 iIndent)
{
  _static_
  ASSERT(bValidate(pNode));

  QueryNode* pExpr = static_cast<QueryNode*>(pNode);

  // Manual polymorphism here

  // Statement
  if      (Statement::bIsOfType(pNode))     { Statement::vExplain(pNode, sBuf, iIndent);    }

    // SqlOperator
  else if (SqlOperator::bIsOfType(pNode))   { SqlOperator::vExplain(pNode, sBuf, iIndent);  }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 PlanObject::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_PLANOBJECT_DEFN);
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

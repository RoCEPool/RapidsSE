/*============================================================================*/
/* JoinType.hpp - an abstract base class of join types used in the plan       */
/*============================================================================*/

#include "BASE.h"
#include "JoinType.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"

#include "InnerJoin.hpp"
#include "CrossJoin.hpp"
#include "NaturalInnerJoin.hpp"
#include "NaturalLeftOuterJoin.hpp"
#include "NaturalRightOuterJoin.hpp"
#include "NaturalFullOuterJoin.hpp"
#include "SemiJoin.hpp"
#include "AntiJoin.hpp"
#include "LeftOuterJoin.hpp"
#include "RightOuterJoin.hpp"
#include "FullOuterJoin.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_JOINTYPE_DEFN = "/__root__/definitions/types/joinType"_Stz;


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Create a type from enum with default properties                            */
/*----------------------------------------------------------------------------*/
QueryNode* JoinType::pCreate(eJT eJoinType)
{
  _static_

  // Manual Polymorphism
  switch (eJoinType) {
    case JT_INNER:
      return _pQueryNode(InnerJoin::pCreate());

    case JT_CROSS:
      return _pQueryNode(CrossJoin::pCreate());

    case JT_SEMI:
      return _pQueryNode(SemiJoin::pCreate());

    case JT_ANTI:
      return _pQueryNode(AntiJoin::pCreate());

    case JT_LEFT_OUTER:
      return _pQueryNode(LeftOuterJoin::pCreate());

    case JT_RIGHT_OUTER:
      return _pQueryNode(RightOuterJoin::pCreate());

    case JT_FULL_OUTER:
      return _pQueryNode(FullOuterJoin::pCreate());

    case JT_NATURAL_INNER:
      return _pQueryNode(NaturalInnerJoin::pCreate());

    case JT_NATURAL_LEFT_OUTER:
      return _pQueryNode(NaturalLeftOuterJoin::pCreate());

    case JT_NATURAL_RIGHT_OUTER:
      return _pQueryNode(NaturalRightOuterJoin::pCreate());

    case JT_NATURAL_FULL_OUTER:
      return _pQueryNode(NaturalFullOuterJoin::pCreate());

    case JT_NONE:
    case JT_NENUMS:
    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void JoinType::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  UNUSED(pNode);

  i8 iVal = pNode->iValue();
  ASSERT(iVal > JT_NONE);
  ASSERT(iVal < JT_NENUMS);
  sBuf.vConcat(aJT_pretty[iVal]);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 JoinType::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_JOINTYPE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 JoinType::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_JOINTYPE_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the name of a join type given a node                               */
/*----------------------------------------------------------------------------*/
Stz* JoinType::szTypeName(PlanNode *pNode)
{
  ASSERT(bIsOfType(pNode));

  i8 iType = pNode->iValue();
  if (iType > JT_NONE && iType < JT_NENUMS) {
    return aJT[iType];
  }

  eJT eType = JoinType::eJoinType(pNode);
  ASSERT(eType > JT_NONE);
  ASSERT(eType < JT_NENUMS);
  return aJT[eType];
}

/*----------------------------------------------------------------------------*/
/* Returns the enum of a join type given a node                               */
/*----------------------------------------------------------------------------*/
eJT JoinType::eJoinType(PlanNode* pNode)
{
  ASSERT(bIsOfType(pNode));

  i8 iType = pNode->iValue();
  if (iType > JT_NONE && iType < JT_NENUMS) {
    return static_cast<eJT>(iType);
  }

  // Manual Polymorphism
  if      (            InnerJoin::bIsOfType(pNode))  { return JT_INNER;               }
  else if (            CrossJoin::bIsOfType(pNode))  { return JT_CROSS;               }
  else if (             SemiJoin::bIsOfType(pNode))  { return JT_SEMI;                }
  else if (             AntiJoin::bIsOfType(pNode))  { return JT_ANTI;                }
  else if (        LeftOuterJoin::bIsOfType(pNode))  { return JT_LEFT_OUTER;          }
  else if (       RightOuterJoin::bIsOfType(pNode))  { return JT_RIGHT_OUTER;         }
  else if (        FullOuterJoin::bIsOfType(pNode))  { return JT_FULL_OUTER;          }
  else if (     NaturalInnerJoin::bIsOfType(pNode))  { return JT_NATURAL_INNER;       }
  else if ( NaturalLeftOuterJoin::bIsOfType(pNode))  { return JT_NATURAL_LEFT_OUTER;  }
  else if (NaturalRightOuterJoin::bIsOfType(pNode))  { return JT_NATURAL_RIGHT_OUTER; }
  else if ( NaturalFullOuterJoin::bIsOfType(pNode))  { return JT_NATURAL_FULL_OUTER;  }
  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

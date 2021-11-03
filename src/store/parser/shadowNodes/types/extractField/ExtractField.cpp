/*============================================================================*/
/* ExtractField.hpp - The fields that can be used in an EXTRACT expression    */
/*============================================================================*/

#include "BASE.h"
#include "store/parser/shadowNodes/types/extractField/ExtractField.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "GraphNav.hpp"

#include "ExtractYear.hpp"
#include "ExtractMonth.hpp"
#include "ExtractDay.hpp"
#include "ExtractHour.hpp"
#include "ExtractMinute.hpp"
#include "ExtractSecond.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_DEFN = "/__root__/definitions/types/extractField"_Stz;

static Stz* acEF[Parser::ExtractField::EF_NENUMS] = {
    nullptr,
    "YEAR"_Stz,
    "MONTH"_Stz,
    "DAY"_Stz,
    "HOUR"_Stz,
    "MINUTE"_Stz,
    "SECOND"_Stz
};

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Creates a extractField node based on its enum type                         */
/*----------------------------------------------------------------------------*/
QueryNode* Parser::ExtractField::pCreate(eEF eType)
{
  _static_

  switch (eType) {

    case EF_YEAR:
      return ExtractYear::pCreate();

    case EF_MONTH:
      return ExtractMonth::pCreate();

    case EF_DAY:
      return ExtractDay::pCreate();

    case EF_HOUR:
      return ExtractHour::pCreate();

    case EF_MINUTE:
      return ExtractMinute::pCreate();

    case EF_SECOND:
      return ExtractSecond::pCreate();

    default:
      NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void Parser::ExtractField::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (  ExtractYear::bIsOfType(pNode))  {   ExtractYear::vDescribe(pNode, sBuf, pRootNode); }
  else if ( ExtractMonth::bIsOfType(pNode))  {  ExtractMonth::vDescribe(pNode, sBuf, pRootNode); }
  else if (   ExtractDay::bIsOfType(pNode))  {    ExtractDay::vDescribe(pNode, sBuf, pRootNode); }
  else if (  ExtractHour::bIsOfType(pNode))  {   ExtractHour::vDescribe(pNode, sBuf, pRootNode); }
  else if (ExtractMinute::bIsOfType(pNode))  { ExtractMinute::vDescribe(pNode, sBuf, pRootNode); }
  else if (ExtractSecond::bIsOfType(pNode))  { ExtractSecond::vDescribe(pNode, sBuf, pRootNode); }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 Parser::ExtractField::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 Parser::ExtractField::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns an enum that represents the type of extractField                    */
/*----------------------------------------------------------------------------*/
Parser::ExtractField::eEF Parser::ExtractField::eExtractFieldType(PlanNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (  ExtractYear::bIsOfType(pNode))  { return EF_YEAR;   }
  else if ( ExtractMonth::bIsOfType(pNode))  { return EF_MONTH;  }
  else if (   ExtractDay::bIsOfType(pNode))  { return EF_DAY;    }
  else if (  ExtractHour::bIsOfType(pNode))  { return EF_HOUR;   }
  else if (ExtractMinute::bIsOfType(pNode))  { return EF_MINUTE; }
  else if (ExtractSecond::bIsOfType(pNode))  { return EF_SECOND; }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* Returns an enum that represents the type of extractField                       */
/*----------------------------------------------------------------------------*/
Stz* Parser::ExtractField::szExtractFieldType(PlanNode* pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  // Manual Polymorphism
  if      (  ExtractYear::bIsOfType(pNode))  { return acEF[EF_YEAR];   }
  else if ( ExtractMonth::bIsOfType(pNode))  { return acEF[EF_MONTH];  }
  else if (   ExtractDay::bIsOfType(pNode))  { return acEF[EF_DAY];    }
  else if (  ExtractHour::bIsOfType(pNode))  { return acEF[EF_HOUR];   }
  else if (ExtractMinute::bIsOfType(pNode))  { return acEF[EF_MINUTE]; }
  else if (ExtractSecond::bIsOfType(pNode))  { return acEF[EF_SECOND]; }

  else {
    // Need to add in manual polymorphism here
    NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

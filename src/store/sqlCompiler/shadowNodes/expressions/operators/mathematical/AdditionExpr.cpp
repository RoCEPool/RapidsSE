/*============================================================================*/
/* AdditionExpr.hpp - an addition (+) expression operator                     */
/*============================================================================*/

#include "BASE.h"
#include "AdditionExpr.hpp"

#include "PlanNode.hpp"
#include "QueryNode.hpp"
#include "eCT.h"
#include "NodeNamer.hpp"
#include "SubqueryRefExpr.hpp"
#include "TypeSystem.hpp"
#include "TypeMap2D.hpp"

using namespace Parser;

/*----------------------------------------------------------------------------*/
/*-- Constants ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
static Stz* C_ADDITION_NAME_PREFIX = "AdditionExpr"_Stz;
static Stz* C_ADDITION_DEFN = "/__root__/definitions/object/expr/operator/mathematical/addition"_Stz;

// What is the resulting type when we combine these inputs?
//  - The first parameter represents the type of the first operand
//  - The second parameter represents the type of the second operand
//  - The third parameter represents the type of the result from this operation.
//  - Any unspecified mappings are unsupported.
//  - Multiple mappings may be made in one go using the eTM values to represent a set of types.
static TypeMap2D* S_pTypeMap = (new TypeMap2D())
    ->pAddSelfMapping(TypeId::xIdOf(CT_DECIMAL))
    ->pAddMapping(TypeId::xIdOf(CT_DECIMAL),        TypeId::xIdOf(CT_UNKNOWN),            TypeId::xIdOf(CT_UNKNOWN))

    ->pAddSelfMapping(TypeId::xIdOf(CT_FLOAT))
    ->pAddMapping(TypeId::xIdOf(CT_FLOAT),          TypeId::xIdOf(CT_UNKNOWN),            TypeId::xIdOf(CT_UNKNOWN))

    ->pAddSelfMapping(TypeId::xIdOf(CT_INTEGER))
    ->pAddMapping(TypeId::xIdOf(CT_INTEGER),        TypeId::xIdOf(CT_UNKNOWN),            TypeId::xIdOf(CT_UNKNOWN))

    // Date
    // A date plus a year-month interval will result in a date.
    // A date plus a day-time interval will result in a timestamp, even if it is
    // a DATE + INTERVAL DAY. (Previously DATE + INTERVAL DAY gave a DATE). This is
    // because the execution engine cannot can only see that the interval is a day-time
    // interval and not its specific type. Hence DATE + day-time intervals must be
    // handled in the same way.
    ->pAddMapping(TypeId::xIdOf(CT_DATE),           TM_ALL_YM_INTERVALS,                  TypeId::xIdOf(CT_DATE))
    ->pAddMapping(TypeId::xIdOf(CT_DATE),           TM_ALL_DT_INTERVALS,                  TypeId::xIdOf(CT_TIMESTAMP))

    // Time
    // A time can only be added with all day-time intervals but not year-month intervals.
    // The day field is effectively ignored because it always adds 24 hours
    // and time is modulo-24 hours.
    ->pAddMapping(TypeId::xIdOf(CT_TIME),            TM_ALL_DT_INTERVALS,                 TypeId::xIdOf(CT_TIME))

    // Timestamp
    ->pAddMapping(TypeId::xIdOf(CT_TIMESTAMP),       TM_ALL_YM_INTERVALS,                 TypeId::xIdOf(CT_TIMESTAMP))
    ->pAddMapping(TypeId::xIdOf(CT_TIMESTAMP),       TM_ALL_DT_INTERVALS,                 TypeId::xIdOf(CT_TIMESTAMP))

    // Intervals + Date
    // Refer to note above about Date + Interval
    ->pAddMapping(TM_ALL_YM_INTERVALS,                  TypeId::xIdOf(CT_DATE),           TypeId::xIdOf(CT_DATE))
    ->pAddMapping(TM_ALL_DT_INTERVALS,                  TypeId::xIdOf(CT_DATE),           TypeId::xIdOf(CT_TIMESTAMP))

    // Intervals + Time
    ->pAddMapping(TM_ALL_DT_INTERVALS,                TypeId::xIdOf(CT_TIME),             TypeId::xIdOf(CT_TIME))

    // Intervals + Timestamp
    ->pAddMapping(TM_ALL_YM_INTERVALS,                TypeId::xIdOf(CT_TIMESTAMP),        TypeId::xIdOf(CT_TIMESTAMP))
    ->pAddMapping(TM_ALL_DT_INTERVALS,                TypeId::xIdOf(CT_TIMESTAMP),        TypeId::xIdOf(CT_TIMESTAMP))

    // Intervals + other Intervals
    // There are lots of combinations here. Resulting type should contain the most and least significant
    // fields from both operands.
    // YM and DT intervals are not compatible.
    ->pAddMapping(TypeId::xIdOf(IV_YEAR),             TypeId::xIdOf(IV_YEAR),             TypeId::xIdOf(IV_YEAR))
    ->pAddMapping(TypeId::xIdOf(IV_YEAR),             TypeId::xIdOf(IV_MONTH),            TypeId::xIdOf(IV_YEAR_TO_MONTH))
    ->pAddMapping(TypeId::xIdOf(IV_YEAR),             TypeId::xIdOf(IV_YEAR_TO_MONTH),    TypeId::xIdOf(IV_YEAR_TO_MONTH))

    ->pAddMapping(TypeId::xIdOf(IV_MONTH),            TypeId::xIdOf(IV_YEAR),             TypeId::xIdOf(IV_YEAR_TO_MONTH))
    ->pAddMapping(TypeId::xIdOf(IV_MONTH),            TypeId::xIdOf(IV_MONTH),            TypeId::xIdOf(IV_MONTH))
    ->pAddMapping(TypeId::xIdOf(IV_MONTH),            TypeId::xIdOf(IV_YEAR_TO_MONTH),    TypeId::xIdOf(IV_YEAR_TO_MONTH))

    ->pAddMapping(TypeId::xIdOf(IV_YEAR_TO_MONTH),    TypeId::xIdOf(IV_YEAR),             TypeId::xIdOf(IV_YEAR_TO_MONTH))
    ->pAddMapping(TypeId::xIdOf(IV_YEAR_TO_MONTH),    TypeId::xIdOf(IV_MONTH),            TypeId::xIdOf(IV_YEAR_TO_MONTH))
    ->pAddMapping(TypeId::xIdOf(IV_YEAR_TO_MONTH),    TypeId::xIdOf(IV_YEAR_TO_MONTH),    TypeId::xIdOf(IV_YEAR_TO_MONTH))

    ->pAddMapping(TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY))
    ->pAddMapping(TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_DAY_TO_HOUR))
    ->pAddMapping(TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_HOUR))
    ->pAddMapping(TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_DAY_TO_SECOND))

    ->pAddMapping(TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY_TO_HOUR))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_HOUR))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_HOUR_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_HOUR))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_HOUR_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_HOUR_TO_SECOND))
    
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_HOUR_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_MINUTE_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_HOUR))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_HOUR_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_MINUTE_TO_SECOND))
    
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_MINUTE_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_MINUTE_TO_SECOND))

    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY_TO_HOUR))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_DAY_TO_HOUR))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_HOUR))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_DAY_TO_SECOND))

    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_DAY_TO_SECOND))

    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_DAY_TO_SECOND))

    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_HOUR_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_HOUR_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_HOUR_TO_MINUTE))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_HOUR_TO_SECOND))

    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_HOUR_TO_SECOND))

    ->pAddMapping(TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_DAY),              TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_HOUR),             TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_MINUTE),           TypeId::xIdOf(IV_MINUTE_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_SECOND),           TypeId::xIdOf(IV_MINUTE_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_DAY_TO_HOUR),      TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_DAY_TO_MINUTE),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_DAY_TO_SECOND),    TypeId::xIdOf(IV_DAY_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_HOUR_TO_MINUTE),   TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_HOUR_TO_SECOND),   TypeId::xIdOf(IV_HOUR_TO_SECOND))
    ->pAddMapping(TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_MINUTE_TO_SECOND), TypeId::xIdOf(IV_MINUTE_TO_SECOND))

    // Intervals + NULLs
    ->pAddMapping(TM_ALL_YM_INTERVALS,                TypeId::xIdOf(CT_UNKNOWN),          TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TM_ALL_DT_INTERVALS,                TypeId::xIdOf(CT_UNKNOWN),          TypeId::xIdOf(CT_UNKNOWN))

    // Nulls
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),          TypeId::xIdOf(CT_DECIMAL),          TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),          TypeId::xIdOf(CT_FLOAT),            TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),          TypeId::xIdOf(CT_INTEGER),          TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),          TM_ALL_YM_INTERVALS,                TypeId::xIdOf(CT_UNKNOWN))
    ->pAddMapping(TypeId::xIdOf(CT_UNKNOWN),          TM_ALL_DT_INTERVALS,                TypeId::xIdOf(CT_UNKNOWN))
    ->pAddSelfMapping(TypeId::xIdOf(CT_UNKNOWN))
;

// References:
//  Exact numeric + exact numeric = exact numeric
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 251, point (1)
//
//  Approximate numeric + exact numeric (any order) = approximate numeric:
//    - SQL standard 2008 (draft): Part 2 (Foundation), page 251, point (2)
//
//  Datetime & Intervals arithmetic:
//    Date      +/-     Interval  = Date
//    Time      +/-     Interval  = Time
//    Timestamp +/-     Interval  = Timestamp
//    Date      -       Date      = Interval_DT
//    Time      -       Time      = Interval_DT
//    Timestamp -       Timestamp = Interval_DT
//    Interval  +       Date      = Date
//    Interval  +       Time      = Time
//    Interval  +       Timestamp = Timestamp
//    Interval  +/-     Interval  = Interval
//    Interval  * or /  Numeric   = Interval
//    Numeric   *       Interval  = Interval
//     - SQL standard 2008 (draft): Part 2 (Foundation), page 39 (table 7)
//
//  Possible enhancements:
//  Postgres seems to support other enhancements, marked as /*?*/ in the table above:
//    Date      +       Time      = Timestamp
//    Date      +       Integer   = Date  (integer is assumed to be in days)
//    Time      +       Date      = Timestamp

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

QueryNode* AdditionExpr::pCreate(QueryNode* pExpr1, QueryNode* pExpr2)
{
  _static_

  // Create a unique name for this object
  s256 sName;
  NodeNamer::vGenerateName(C_ADDITION_DEFN, sName);

  QueryNode* pExpr = new QueryNode(sName, C_ADDITION_DEFN);
  pExpr->vAppendChild(pExpr1);
  pExpr->vAppendChild(pExpr2);

  return pExpr;
}


/*----------------------------------------------------------------------------*/
/* Appends a basic, single line description of the object into a buffer       */
/*----------------------------------------------------------------------------*/
void AdditionExpr::vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = AdditionExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = AdditionExpr::pChild2(_pQueryNode(pNode));

  if (pChild1->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild1)) {
    Expr::vDescribe(pChild1, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild1, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }

  sBuf.vConcat(" + "_Stz);

  if (pChild2->nChildren() == 0 || SubqueryRefExpr::bIsOfType(pChild2)) {
    Expr::vDescribe(pChild2, sBuf, pRootNode);

  } else {
    sBuf.vConcat("("_Stz);
    Expr::vDescribe(pChild2, sBuf, pRootNode);
    sBuf.vConcat(")"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* Represent the expression as types instead of names. Append to the buffer   */
/*----------------------------------------------------------------------------*/
void AdditionExpr::vDescribeAsTypes(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pNode));

  QueryNode* pChild1 = AdditionExpr::pChild1(_pQueryNode(pNode));
  QueryNode* pChild2 = AdditionExpr::pChild2(_pQueryNode(pNode));

  sBuf.vConcat(Expr::xTypeId(pChild1, pRootNode).szTypeName());
  sBuf.vConcat(" + "_Stz);
  sBuf.vConcat(Expr::xTypeId(pChild2, pRootNode).szTypeName());
}

/*----------------------------------------------------------------------------*/
/* Return the first child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* AdditionExpr::pChild1(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pFirstChild());
}

/*----------------------------------------------------------------------------*/
/* Return the second child expression of this operator                         */
/*----------------------------------------------------------------------------*/
QueryNode* AdditionExpr::pChild2(QueryNode *pNode)
{
  _static_
  ASSERT(bIsOfType(pNode));
  ASSERT(pNode->nChildren() == 2);

  return _pQueryNode(pNode->pSecondChild());
}


/*----------------------------------------------------------------------------*/
/* Returns true if the given node is derived from the expr definition         */
/*----------------------------------------------------------------------------*/
b1 AdditionExpr::bIsOfType(PlanNode *pNode)
{
  return ShadowNode::bIsOfType(pNode, C_ADDITION_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns true if the given node is precisely the same as its definition     */
/*----------------------------------------------------------------------------*/
b1 AdditionExpr::bIsExactType(PlanNode *pNode)
{
  return ShadowNode::bIsExactType(pNode, C_ADDITION_DEFN);
}

/*----------------------------------------------------------------------------*/
/* Returns the resulting type of the expression operation.                    */
/* A returned value of CT_NONE means that it is not supported or invalid.     */
/*----------------------------------------------------------------------------*/
TypeId AdditionExpr::xTypeId(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));

  // Get the types of the two operands
  QueryNode* pChild1 = AdditionExpr::pChild1(pExprNode);
  QueryNode* pChild2 = AdditionExpr::pChild2(pExprNode);

  TypeId xType1 = Expr::xTypeId(pChild1, pRootNode);
  TypeId xType2 = Expr::xTypeId(pChild2, pRootNode);

  // Lookup the type of the result
  TypeId xResult = S_pTypeMap->xLookup(xType1, xType2);
  return xResult;
}

/*----------------------------------------------------------------------------*/
/* Returns the precision of the resulting expression or -1 if not set or n/a  */
/*----------------------------------------------------------------------------*/
i8 AdditionExpr::iPrecision(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);

  TypeId xResultType = xTypeId(pExprNode, pRootNode);
  eCT eResultType = xResultType.eCTVal();

  i8 iVal = -1;

  switch (eResultType) {

    case CT_NONE:
    case CT_BOOLEAN:
    case CT_CHAR:
    case CT_DATE:
      break;

    case CT_DECIMAL:
      iVal = iCalcDecimalPrecision(pChild1(pExprNode), pChild2(pExprNode), pRootNode);
      break;

    case CT_FLOAT:
      iVal = C_MAX_FLT_PRECISION;
      break;

    case CT_INTEGER:
      iVal = C_MAX_INT_PRECISION;
      break;

    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
    case CT_UNKNOWN:
      break;

    case CT_ENUMS:
      NEVER_REACH;
  }

  return iVal;
}

/*----------------------------------------------------------------------------*/
/* Returns the scale of the resulting expression or -1 if not set or n/a      */
/*----------------------------------------------------------------------------*/
i8 AdditionExpr::iScale(QueryNode* pExprNode, PlanNode* pRootNode)
{
  _static_
  ASSERT(bIsOfType(pExprNode));
  ASSERT(pRootNode);

  TypeId xResultType = xTypeId(pExprNode, pRootNode);
  eCT eResultType = xResultType.eCTVal();

  i8 iVal = -1;

  switch (eResultType) {

    case CT_NONE:
    case CT_BOOLEAN:
    case CT_CHAR:
    case CT_DATE:
      break;

    case CT_DECIMAL:
      iVal = iCalcDecimalScale(pChild1(pExprNode), pChild2(pExprNode), pRootNode);
      break;

    case CT_FLOAT:
    case CT_INTEGER:
    case CT_INTERVAL_DT:
    case CT_INTERVAL_YM:
    case CT_TIME:
    case CT_TIMESTAMP:
    case CT_VARCHAR:
    case CT_UNKNOWN:
      break;

    case CT_ENUMS:
      NEVER_REACH;
  }

  return iVal;
}


/*----------------------------------------------------------------------------*/
/* Calculates the resulting precision of adding two decimal operands          */
/*----------------------------------------------------------------------------*/
i8 AdditionExpr::iCalcDecimalPrecision(Parser::QueryNode *pChild1,
                                       Parser::QueryNode *pChild2,
                                       Parser::PlanNode *pRootNode)
{
  _static_
  ASSERT(pChild1);
  ASSERT(pChild2);
  ASSERT(pRootNode);
  ASSERT(Expr::xTypeId(pChild1,pRootNode) == TypeId::xIdOf(CT_DECIMAL));
  ASSERT(Expr::xTypeId(pChild2,pRootNode) == TypeId::xIdOf(CT_DECIMAL));

  // Precision = max(s1,s2) + max(p1 - s1, p2 - s2) + 1

  i8 iPrecision1 = Expr::iPrecision(pChild1, pRootNode);
  i8 iPrecision2 = Expr::iPrecision(pChild2, pRootNode);
  i8 iScale1     = Expr::iScale(pChild1, pRootNode);
  i8 iScale2     = Expr::iScale(pChild2, pRootNode);

  ASSERT(iPrecision1 > 0);
  ASSERT(iPrecision2 > 0);
  ASSERT(iScale1 >= 0);
  ASSERT(iScale2 >= 0);

  i8 iVal = MAX__(iScale1, iScale2) + MAX__((iPrecision1 - iScale1), (iPrecision2 - iScale2)) + 1;

  if (iVal > C_MAX_BCD_PRECISION) {
    iVal = C_MAX_BCD_PRECISION;
  }

  return iVal;
}
/*----------------------------------------------------------------------------*/
/* Calculates the resulting scale of adding two decimal operands             */
/*----------------------------------------------------------------------------*/
i8 AdditionExpr::iCalcDecimalScale(Parser::QueryNode *pChild1,
                                   Parser::QueryNode *pChild2,
                                   Parser::PlanNode *pRootNode)
{
  _static_
  ASSERT(pChild1);
  ASSERT(pChild2);
  ASSERT(pRootNode);
  ASSERT(Expr::xTypeId(pChild1,pRootNode) == TypeId::xIdOf(CT_DECIMAL));
  ASSERT(Expr::xTypeId(pChild2,pRootNode) == TypeId::xIdOf(CT_DECIMAL));

  // Scale = max(s1,s2)

  i8 iScale1     = Expr::iScale(pChild1, pRootNode);
  i8 iScale2     = Expr::iScale(pChild2, pRootNode);

  ASSERT(iScale1 >= 0);
  ASSERT(iScale2 >= 0);

  i8 iVal = MAX__(iScale1, iScale2);

  if (iVal > C_MAX_BCD_SCALE) {
    iVal = C_MAX_BCD_SCALE;
  }

  return iVal;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

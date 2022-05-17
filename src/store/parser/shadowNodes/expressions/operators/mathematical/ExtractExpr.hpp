/*============================================================================*/
/* ExtractExpr.hpp - an EXTRACT expression operator                           */
/*============================================================================*/
#pragma once

#include "MathematicalExpr.hpp"
#include "ExtractField.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ExtractExpr : public MathematicalExpr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ExtractExpr();

  public:
    static QueryNode* pCreate(QueryNode* pExtractField, QueryNode* pExpr);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // The enum field to be extracted
    static ExtractField::eEF eExtractField(QueryNode* pNode);

    // The expression that we are extracting the field from
    static QueryNode* pSourceExpr(QueryNode* pNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1   bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation.
    // A returned value of CT_NONE means that it is not supported or invalid.
    static i8 iType(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

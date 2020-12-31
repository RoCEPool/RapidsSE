/*============================================================================*/
/* ColumnRef.hpp - an expression representing a reference to a column         */
/*============================================================================*/
#pragma once

#include "Expr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ColumnRef : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ColumnRef();

  public:
    static QueryNode* pCreate(Text* pCatalogName, Text* pSchemaName,
                              Text* pTableName, Text* pColumnName);

    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

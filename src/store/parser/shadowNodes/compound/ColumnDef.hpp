/*============================================================================*/
/* ColumnDef.hpp - a definition of a column used in the plan                  */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "SqlType.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ColumnDef : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    ColumnDef();

  public:
    static QueryNode* pCreate(Stz* szColumnName, SqlType::Type eColumnType, b1 bNotNull);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    static Stz*           szColumnName(QueryNode* pNode);
    static SqlType::Type eColumnType(QueryNode* pNode);
    static b1             bNotNull(QueryNode* pNode);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

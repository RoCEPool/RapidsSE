/*============================================================================*/
/* ColumnDefs.hpp - a list of columns definitions                             */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ColumnDefs : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    ColumnDefs();

  public:
    static QueryNode* pCreate();
    static void       vAddColumnDef(QueryNode* pNode, QueryNode* pColumnDef);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static i8 nColumns(QueryNode* pColumnDefs);

    // Returns a pointer to an element in this list that matches the given
    // column ordinal number or name. Returns nullptr if not found.
    static QueryNode* pFind(QueryNode* pNode, i8 iColumnOrdinal);
    static QueryNode* pFind(QueryNode* pNode, Stz* iColumnName);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

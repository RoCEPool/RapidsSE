/*============================================================================*/
/* TableScanOp.hpp - an operator that linearly reads all rows in a table      */
/*============================================================================*/
#pragma once

#include "DataSource.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class QueryNode;

  class TableScanOp : public DataSource
  {
  private:
    // Shalt never be instantiated. Static functions only.
    TableScanOp();

  public:
    static QueryNode* pCreate(QueryNode* pTableRef);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static QueryNode* pTableRef(QueryNode* pNode);

    static Stz* szCatalogName(QueryNode* pNode);
    static Stz* szSchemaName(QueryNode* pNode);
    static Stz* szTableName(QueryNode* pNode);

    // Get the list of column definitions for this data source
    // Returned list must be freed, but not its contents.
    static PlanNodeList* pColumnDefs(QueryNode* pNode, PlanNode* pRootNode);

    // Return a list of ProjectedExprs that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static PlanNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

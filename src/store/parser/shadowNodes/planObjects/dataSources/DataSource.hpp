/*============================================================================*/
/* DataSource.hpp - a source of row data into the query                       */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class DataSource : public SqlOperator
  {

  private:
    // Shalt never be instantiated. Static functions only.
    DataSource();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Get the catalog/schema/table name of the data source, if applicable
    static Stz* szCatalogName(QueryNode* pNode);
    static Stz* szSchemaName(QueryNode* pNode);
    static Stz* szTableName(QueryNode* pNode);

    // Get the list of column definitions for this data source
    // Returned list must be freed, but not its contents.
    static PlanNodeList* pColumnDefs(QueryNode* pNode, PlanNode* pRootNode);

    // Return a list of ProjectedExprs that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static PlanNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

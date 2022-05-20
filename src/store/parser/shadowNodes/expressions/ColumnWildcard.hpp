/*============================================================================*/
/* ColumnWildcard.hpp - an expression representing one or more columns        */
/* This object will only exist temporarily until all tables in the plan have  */
/* been resolved. Then this will be replaced by a set of columnRefs instead.  */
/*============================================================================*/
#pragma once

#include "Expr.hpp"
#include "TypeId.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class ColumnWildcard : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ColumnWildcard();

  public:
    static QueryNode* pCreate(Text* pCatalogName, Text* pSchemaName, Text* pTableName);

    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1    bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static TypeId xTypeId(QueryNode* pNode, PlanNode* pRootNode);

    static Stz*  szCatalog(QueryNode* pColumnWildcard);
    static Stz*  szSchema(QueryNode* pColumnWildcard);
    static Stz*  szTable(QueryNode* pColumnWildcard);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

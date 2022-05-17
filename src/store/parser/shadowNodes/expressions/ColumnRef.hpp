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
    static QueryNode* pCreate(Text* pTableAliasName,
                              Text* pCatalogName, Text* pSchemaName,
                              Text* pTableName, Text* pColumnName,
                              Text* pDefnPath = nullptr);

    static QueryNode* pCreate(Stz* szTableAliasName,
                              Stz* szCatalogName, Stz* szSchemaName,
                              Stz* szTableName,   Stz* szColumnName,
                              Stz* pDefnPath = nullptr);

    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1    bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static i8 iType(QueryNode* pNode, PlanNode* pRootNode);

    static Stz*  szTableAlias(QueryNode* pColumnRef);
    static Stz*  szCatalog(QueryNode* pColumnRef);
    static Stz*  szSchema(QueryNode* pColumnRef);
    static Stz*  szTable(QueryNode* pColumnRef);
    static Stz*  szColumn(QueryNode* pColumnRef);
    static Stz*  szDefnPath(QueryNode* pColumnRef); // may be null
    static b1    bIsFullyQualified(QueryNode* pColumnRef);
    static void  vQualify(QueryNode* pColumnRef, Stz* szTableAliasName,
                          Stz* szCatalogName,    Stz* szSchemaName,
                          Stz* szTableName,      Stz* szColumnName,
                          Stz* szDefnPath);

    // Compare two expression trees and return true if they are equal with
    // respect to the operations and symbols used.
    // This is overloaded from Expr to provide case insensitive property comparisons
    static b1 bIsSymbolicallyEqual(QueryNode* pExpr1, QueryNode* pExpr2);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

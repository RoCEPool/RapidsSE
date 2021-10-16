/*============================================================================*/
/* AliasRef.hpp - an expression representing a reference to an alias          */
/*============================================================================*/
#pragma once

#include "Expr.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class AliasRef : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    AliasRef();

  public:
    static QueryNode* pCreate(Text* pAliasName, QueryNode* pAliasExpr);

    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1    bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static eCT   eType(QueryNode* pNode, PlanNode* pRootNode);

    // Return the name of the alias
    static Stz*  szAlias(QueryNode* pNode);

    // Return the expression that is being aliased
    static QueryNode* pAliasedExpr(QueryNode* pNode);

    // Compare two expression trees and return true if they are equal with
    // respect to the operations and symbols used.
    // This is overloaded from Expr to provide case insensitive property comparisons
    static b1 bIsSymbolicallyEqual(QueryNode* pAliasRef1, QueryNode* pAliasRef2);

    // Replace the current AliasRef node with its expanded expression, in place.
    static void vExpandAlias(QueryNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

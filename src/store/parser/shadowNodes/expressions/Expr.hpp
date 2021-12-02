/*============================================================================*/
/* Expr.hpp - a generic expression                                            */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  class Expr : public ShadowNode
  {
    // A note on Expr subclasses:
    // **************************
    //
    // All subclasses of Expr should be built such that all parameters that themselves
    // can be other forms of expressions MUST be placed as children and not as properties
    // of a node. Other parameters that can not be represented as general expressions CAN
    // be stored as a property.
    //
    // The reason for this is that many of the utility functions below (e.g. bContainsAnAggregate,
    // or the conversion from symbolic to ordinal representation) walk the expression graph
    // and inspect the contents. By storing all expr operands as children this is drastically
    // simplified and any complications are contained to the individual expression operators themselves.
    //
    // Example 1: an AND operator will have two expr operands represented as children.
    // Example 2: an EXTRACT operator can store the extraction field (YEAR, MONTH, etc)
    //            as a property as this is a fixed string (not an expr) but the expr
    //            it is extracting that field from must be stored as a child.

  private:
    // Shalt never be instantiated. Static functions only.
    Expr();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Returns the resulting type of the expression operation
    static eCT eType(QueryNode* pExprNode, PlanNode* pRootNode);

    // Returns true if this expression (and therefore any subexpression elements
    // it contains) is an aggregate expression. It may also contain non-aggregated
    // terms too.
    static b1 bContainsAnAggregate(PlanNode* pNode);

    // Returns true if this expression (and any subexpression elements it contains)
    // has a ColumnRef that is not part of an aggregation. If true, this is not to
    // say that this expression tree doesn't contain aggregations, it could, but
    // it may also contain non-aggregated ColumnRefs too. e.g. the expression
    // "col1 + SUM(col2)" contains both an aggregate term and a non-aggregated
    // ColumnRef.
    static b1 bContainsNonaggregatedColumnRefs(PlanNode* pNode);

    // Returns true if this expression tree only comprises literals and operators.
    // i.e., returns false if a ColumnRef or ColumnOrdinal is encountered.
    static b1 bIsFullyLiteral(PlanNode* pNode);

    // Checks the expression tree to determine that a supported result type
    // can be produced. Will throw a SqlException if it is an invalid expression.
    static void vValidateResultType(QueryNode* pExprNode, PlanNode* pRootNode);

    // Takes a symbolic expression and returns a copy of it that is relative to
    // column ordinals for a given list of column expressions.
    //   e.g. given the symbolic expression SUM(COUNT(DISTINCT colA) + 1)
    //   and the fact that COUNT(DISTINCT colA) is already being returned
    //   by a child operator as the 3rd column, then this function would
    //   return the ordinal expression: SUM($3 + 1)
    // The returned QueryNode must be freed.
    // Column ordinals are 1-based.
    static QueryNode* pSymbolicToOrdinal(QueryNode*     pSymbolicExpr,
                                         QueryNodeList* pColumnExprList,
                                         QueryNode*     pCorrelatedExprs,
                                         PlanNode*      pRootNode);

    // Takes an ordinal expression and returns a symbolic copy of it based on the
    // column expressions projected from a child sql operator.
    // The returned QueryNode must be freed.
    static QueryNode* pOrdinalToSymbolic(QueryNode*     pOrdinalExpr,
                                         QueryNodeList* pColumnExprList,
                                         PlanNode*      pRootNode);

    // Compare two expression trees and return true if they are equal with
    // respect to the operations and symbols used.
    static b1 bIsSymbolicallyEqual(QueryNode* pExpr1, QueryNode* pExpr2);

    // Compare two expression trees symbolically and return true if
    // pSuperExpr is equal to, or a superset of, pSubExpr.
    static b1 bIsEqualOrSupersetOf(QueryNode* pSuperExpr, QueryNode* pSubExpr);

    // Returns true if the expression does not contain any column references.
    // All expressions are relative column ordinals, operators or literals only.
    static b1 bIsFullyOrdinal(QueryNode* pExpr);

    // Returns true if the expression does not contain any ordinal column
    // references. Can contain (symbolic) ColumnRefs and literals.
    static b1 bIsNotOrdinal(QueryNode* pExpr);

    // Given an expression already in ordinal form, find and return the largest
    // non-ordinal subset of that expression. If the expression is wholly
    // ordinal then nullptr is returned.
    static QueryNode* pLargestNonOrdinalSubExpr(QueryNode* pOrdinalExpr, b1 bIncludeAggregates);

    // Given an expression, return a list of each aggregate term found in
    // that expression. The returned list is to be deleted, but the items
    // in the list are not to be deleted.
    // If bDescendAggregates is true then all nested aggregate terms are returned.
    // If false then only the outer aggregate terms are returned.
    static QueryNodeList* pAggregateExprs(QueryNode* pExpr, b1 bDescendAggregates);

    // Changes the existing expression in place to support
    // super-aggregation of partitioned aggregate expressions.
    static void vSuperAggregate(QueryNode* pExpr);

    // Modifies subexpressions by promoting/casting data types so that they
    // are type compatible with this expression. Modifies the expression
    // tree in place.
    static void vCoerceSubExprs(QueryNode* pExpr, PlanNode* pRootNode);

  protected:
    // Returns true if the expression is commutative (e.g. A+B = B+A)
    // and the underlying expression children are also symbolically equal.
    static b1 bIsCommutativelyEqual(QueryNode* pExpr1, QueryNode* pExpr2);

    // Returns true if the two expressions are not equal but still represent
    // the same logical result. e.g. A<B is symbolically equal to B>A
    static b1 bIsMathematicallyEqual(QueryNode* pExpr1, QueryNode* pExpr2);

    // Find the SqlOperator node that contains this expression instance.
    // Involves a search of the graph, so can be an expensive operation.
    static QueryNode* pFindEnclosingSqlOp(QueryNode* pExpr, PlanNode* pRootNode);

    // Try to find a common type that all children of pExpr can be coerced to.
    // Return true if the coerced and modified pExpr now returns a value for
    // eType() that is not CT_NONE.
    static b1 bCoerceChildrenToCommonType(QueryNode* pExprCopy, PlanNode* pRootNode);

    // Increments the integer elements of an array if the given expression is
    // coercible to each eCT data type.
    static void vIncrementCoercibleTargets(QueryNode* pExpr, i8 (&aiCoercibleTargets)[CT_ENUMS], PlanNode* pRootNode);

    // Given an array of counts for each data types and a maximum count value,
    // choose the data type that should be coerced to, or CT_NONE if none of
    // them reached iMaxCount.
    static eCT eCommonCoercibleTarget(i8 (&aiCoercibleTargets)[CT_ENUMS], i8 iMaxCount);

    // Returns true if an expression can be coerced to a given data type.
    // Currently supported implicit casts include:
    //   current type       -> current type
    //   integer            -> decimal
    //   integer            -> float
    //   decimal            -> float
    //   string (integer)   -> integer     (from a literal only)
    //   string (integer)   -> decimal     (from a literal only)
    //   string (decimal)   -> decimal     (from a literal only)
    //   string (integer)   -> float       (from a literal only)
    //   string (decimal)   -> float       (from a literal only)
    //   string (float)     -> float       (from a literal only)
    //   string             -> date        (from a literal only)
    //   string             -> time        (from a literal only)
    //   string (date)      -> timestamp   (from a literal only)
    //   string (timestamp) -> timestamp   (from a literal only)
    //   date               -> timestamp
    //   boolean            -> string
    static b1 bIsImplicitlyCoercibleTo(eCT eTarget, QueryNode* pExpr, PlanNode* pRootNode);

    // Returns a new expression object which represents pExpr coerced to eTarget
    static QueryNode* pCoerceTo(eCT eTarget, QueryNode* pExpr, PlanNode* pRootNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

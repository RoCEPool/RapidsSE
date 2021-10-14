/*============================================================================*/
/* PlanObject.hpp - an abstract base class of all object that appear in the   */
/* main data path of the plan graph.                                          */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"
#include "store/parser/shadowNodes/types/location/Location.hpp"

namespace Parser
{
  class QueryNode;

  class PlanObject : public ShadowNode
  {

  private:
    // Shalt never be instantiated. Static functions only.
    PlanObject();

  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

    // Return a list of ProjectedColumns that represent the column expressions
    // that are provided by the child(ren) of this PlanObject to this node.
    // Most PlanObjects only have 1 child, but some can have more (e.g. Joins).
    // This method handles that. The list and all contents returned must be freed.
    static QueryNodeList* pInputColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

    // Returns the internal node that represents the list of correlated expressions
    // of the parent subquery of pNode. The returned object is of type ExprList.
    // Do not delete or modify the returned node or its children.
    static QueryNode* pCorrelatedExprs(QueryNode* pNode, PlanNode* pRootNode);

    // Set or clear the location where this planObject is supposed to run in the cluster
    static void vSetLocation(QueryNode* pNode, Location::eLO eLocation);
    static void vClearLocation(QueryNode* pNode);

    // Returns the location of this node, otherwise if the node doesn't have a location
    // then it can descend into the children until a location is found. If no location
    // is set then LO_NONE is returned.
    static Location::eLO eLocation(QueryNode* pNode, b1 bIncludeChildren);

    // Returns true if this specific node has a location property and it's value is valid
    static b1 bNodeHasLocation(QueryNode* pNode);

    // Returns true if this operator (and any expressions it may have) should be
    // partitioned when the plan is partitioned. Most operators will simply ask
    // this question of their child until we get to a data source. Most tables
    // would answer true. A replicated table or data source would answer false.
    static b1 bIsPartitionable(QueryNode* pNode, PlanNode* pRootNode);

  protected:
    // Indent format
    enum eIndentFormat {
      IF_NONE = 0,
      IF_PLAIN,
      IF_ARROW,
      IF_NENUMS
    };

    static void vAppendIndent(s48K& sBuf, i8 iIndentLevel, eIndentFormat eFormat);

    // Returns the first parental SubqueryMarker of a given absolute path, or
    // nullptr if one is not found.
    static QueryNode* pFindEnclosingSubquery(s2K& sPathToSearch, PlanNode* pPathRoot, s2K* psEnclosingSubqueryPath = nullptr);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

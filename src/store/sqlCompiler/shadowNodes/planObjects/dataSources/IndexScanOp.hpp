/*============================================================================*/
/* IndexScanOp.hpp - an operator that linearly reads all rows from an index   */
/*============================================================================*/
#pragma once

#include "TableSource.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  class IndexScanOp : public TableSource
  {
  private:
    // Shalt never be instantiated. Static functions only.
    IndexScanOp();

  public:
    static QueryNode* pCreate(QueryNode* pIndexRef);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

    static QueryNode* pTableRef(QueryNode* pNode);
    static QueryNode* pIndexRef(QueryNode* pNode);
    static Stz*       szIndexName(QueryNode* pNode);


  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

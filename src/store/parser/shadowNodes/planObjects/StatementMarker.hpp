/*============================================================================*/
/* Statement.hpp - identifies the type of query being planned                 */
/*============================================================================*/
#pragma once

#include "PlanObject.hpp"
#include "PlanNode.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class StatementMarker : public PlanObject
  {
  public:
    // Statement types
    enum eST {
      ST_NONE = 0,
      ST_EXPLAIN,
      ST_CREATETABLE,
      ST_DROPTABLE,
      ST_CREATEINDEX,
      ST_DROPINDEX,
      ST_CTAS,
      ST_SELECT,
      ST_INSERT,
      ST_UPDATE,
      ST_DELETE,
      ST_INSERTSELECT,
      ST_NENUMS
    };

  private:
    // Shalt never be instantiated. Static functions only.
    StatementMarker();

  public:
    static QueryNode* pCreate(eST eStmtType, Stz* szQueryText);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static PlanNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

  protected:
    static eST eStatementType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

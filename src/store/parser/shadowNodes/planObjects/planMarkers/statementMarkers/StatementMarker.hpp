/*============================================================================*/
/* StatementMarker.hpp - identifies the type of query being planned           */
/*============================================================================*/
#pragma once

#include "store/parser/shadowNodes/planObjects/planMarkers/PlanMarker.hpp"
#include "PlanNode.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class QueryNode;

  class StatementMarker : public PlanMarker
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

  protected:
    static QueryNode* pCreate(eST eStmtType, Stz* szDefn, Stz* szQueryText);

  public:
    static void vDescribe(PlanNode *pNode, s2K &sBuf, PlanNode *pRootNode);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, QueryNode* pExplainNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, QueryNode* pExplainNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);

  protected:
    static eST eStatementType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

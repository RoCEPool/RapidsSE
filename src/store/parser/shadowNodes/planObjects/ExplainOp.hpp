/*============================================================================*/
/* ExplainOp.hpp - operator that explains the underlying plan                 */
/*============================================================================*/
#pragma once

#include "SqlOperator.hpp"
#include "QueryNodeList.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class ExplainOp : public SqlOperator
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ExplainOp();

  public:
    static QueryNode* pCreate(QueryNode* pStageRangeList, Stz* szFormat, b1 bWithNames, b1 bWithTypes);
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

    // Return a list of ProjectedColumns that represent the column expressions
    // that this PlanObject will return. All nodes and the list must be freed.
    static QueryNodeList* pColumnExprs(QueryNode* pNode, PlanNode* pRootNode);

    // Increments the number of stages (operators in the pipeline) encountered
    static void vIncStageCount(QueryNode* pNode);

    // Get the current stage count
    static i8 iStageCount(QueryNode* pNode);

    // Record the name of a compiler stage encountered
    static void vAddCompilerStageName(QueryNode* pNode, Stz* szStageName);

    // Get the last compiler stage name encountered
    static Stz* szLastStageName(QueryNode* pNode);

    // Returns true if the current stage needs to be explained.
    // Uses the # stages encountered and the list of stage names past to determine this.
    static b1 bExplainStage(QueryNode* pNode);

    // Returns true if we have reached the last stage to be explained
    static b1 bExitEarly(QueryNode* pNode);

    // Return the name of the last explainable stage before transliteration
    static Stz* szLastRegularExplainStage();

    // Return the tabledef name for the definition of the explain table
    // for the current format. Must return one of the C_TDEF_* constants.
    static Stz* szTableDef(QueryNode* pNode);

    // Returns true if the names of the sql operator nodes should be printed instead of their types
    static b1 bWithNames(QueryNode* pNode);

    // Returns true if the types of the projected expressions should be printed
    static b1 bWithTypes(QueryNode* pNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

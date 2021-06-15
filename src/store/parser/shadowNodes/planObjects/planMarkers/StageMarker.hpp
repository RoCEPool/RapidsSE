/*============================================================================*/
/* StageMarker.hpp - indicates to the transliteration when a new pipeline     */
/* stage is required                                                          */
/*============================================================================*/
#pragma once

#include "store/parser/shadowNodes/planObjects/planMarkers/PlanMarker.hpp"

namespace Parser
{
  class QueryNode;

  class StageMarker : public PlanMarker
  {
  private:
    // Shalt never be instantiated. Static functions only.
    StageMarker();

  public:
    static QueryNode* pCreate();
    static void vDescribe(PlanNode* pNode, s2K& sBuf);
    static void vExplain(PlanNode *pNode, PlanNode *pRootNode, s48K &sBuf, i8 iIndent);

    // Explains any common properties of parent definitions
    static void vExplainCommon(PlanNode *pNode, PlanNode* pRootNode, Stz* szType, s48K &sBuf, i8 iIndent);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);
    static b1 bIsOfType(Stz* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);
    static b1 bIsExactType(Stz* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

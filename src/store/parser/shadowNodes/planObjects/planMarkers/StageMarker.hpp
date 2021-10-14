/*============================================================================*/
/* StageMarker.hpp - indicates to the transliteration when a new pipeline     */
/* stage is required                                                          */
/*============================================================================*/
#pragma once

#include "store/parser/shadowNodes/planObjects/planMarkers/PlanMarker.hpp"
#include "store/parser/shadowNodes/types/location/Location.hpp"
#include "store/parser/shadowNodes/types/routing/Routing.hpp"

namespace Parser
{
  class QueryNode;

  class StageMarker : public PlanMarker
  {
  private:
    // Shalt never be instantiated. Static functions only.
    StageMarker();

  public:
    static QueryNode* pCreate(Location::eLO eLocationType, Routing::eRT eRoutingType);
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

    // Returns the location information of the StageMarker.
    // This is where all sql operators below it are to be executed.
    static QueryNode*    pLocation(PlanNode* pNode);
    static Location::eLO eLocation(PlanNode* pNode);
    static Stz*          szLocation(PlanNode* pNode);

    // Returns the routing information of the StageMarker.
    // This is how row data should be routed to the sql operators above it.
    static QueryNode*    pRouting(PlanNode* pNode);
    static Routing::eRT  eRouting(PlanNode* pNode);
    static Stz*          szRouting(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

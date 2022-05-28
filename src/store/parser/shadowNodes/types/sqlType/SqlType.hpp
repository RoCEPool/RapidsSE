/*============================================================================*/
/* SqlType.hpp - an abstract base class of SQL data types used in the plan    */
/*============================================================================*/
#pragma once

#include "Types.hpp"
#include "TypeId.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class SqlType : public Types
  {
  private:
    // Shalt never be instantiated. Static functions only.
    SqlType();


  public:
    static QueryNode* pCreate(TypeId xTypeId);
    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    static Stz* szTypeName(PlanNode* pNode);
    static TypeId xTypeId(PlanNode* pNode);

  protected:
    static void vCreateCommon(QueryNode* pNode, TypeId xTypeId);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* ExtractField.hpp - The fields that can be used in an EXTRACT expression    */
/*============================================================================*/
#pragma once

#include "Types.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class ExtractField : public Types
  {
  private:
    // Shalt never be instantiated. Static functions only.
    ExtractField();


  public:
    enum eEF {
      EF_NONE = 0,
      EF_YEAR,
      EF_MONTH,
      EF_DAY,
      EF_HOUR,
      EF_MINUTE,
      EF_SECOND,
      EF_NENUMS
    };

    static QueryNode* pCreate(eEF eType);

    static void vDescribe(PlanNode* pNode, s2K& sBuf, PlanNode* pRootNode);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    // Returns true if a given node is this exact given type.
    static b1 bIsExactType(PlanNode* pNode);

    // Get an enum that represents the type of extractField
    static eEF eExtractFieldType(PlanNode* pNode);

    // Get a short string that represents the type of extractField
    static Stz* szExtractFieldType(PlanNode* pNode);
    static Stz* szExtractFieldType(eEF eExtractField);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

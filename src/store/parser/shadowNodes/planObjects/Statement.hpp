/*============================================================================*/
/* Statement.hpp - identifies the type of query being planned                 */
/*============================================================================*/
#pragma once

#include "PlanObject.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class Statement : public PlanObject
  {
  public:
    // Statement types
    enum eST {
      ST_NONE = 0,
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
      ST_ENUMS
    };

  private:
    // Shalt never be instantiated. Static functions only.
    Statement();

  public:
    static QueryNode* pCreate(eST eStmtType, Stz* szQueryText);
    static void       vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

  protected:

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

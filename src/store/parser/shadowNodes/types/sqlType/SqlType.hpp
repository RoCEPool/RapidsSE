/*============================================================================*/
/* SqlType.hpp - an abstract base class of SQL data types used in the plan    */
/*============================================================================*/
#pragma once

#include "Types.hpp"

namespace Parser
{
  class PlanNode;
  class QueryNode;

  class SqlType : public Types
  {
  public:
    typedef enum {
      ST_NONE = 0,
      ST_BOOLEAN,
      ST_INTEGER,
      ST_FLOAT,
      ST_DECIMAL,
      ST_VARCHAR,
      ST_DATE,
      ST_TIME,
      ST_TIMESTAMP,
      ST_INTERVAL_YM,
      ST_INTERVAL_DT,
      ST_UNKNOWN,   // for null literals
      ST_NENUMS
    } Type;

  private:
    // Shalt never be instantiated. Static functions only.
    SqlType();


  public:
    static void vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    static Stz* szTypeName(PlanNode* pNode);
    static Type eType(PlanNode* pNode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

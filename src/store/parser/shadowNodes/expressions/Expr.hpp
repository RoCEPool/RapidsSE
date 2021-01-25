/*============================================================================*/
/* Expr.hpp - a generic expression                                            */
/*============================================================================*/
#pragma once

#include "ShadowNode.hpp"

namespace Parser
{
  class QueryNode;
  class PlanNode;

  class Expr : public ShadowNode
  {
  public:
    // Sql Expression Type
    enum eET {
      ET_NONE = 0,
      ET_NULL,
      ET_BOOLEAN,
      ET_STRING,
      ET_INTEGER,
      ET_FLOAT,
      ET_DECIMAL,
      ET_DATE,
      ET_TIME,
      ET_TIMESTAMP,
      ET_INTERVAL,
      ET_NENUMS
    };

  private:
    // Shalt never be instantiated. Static functions only.
    Expr();

  public:
    static Stz* szCatalog(QueryNode* pTableRef);
    static void vDescribe(PlanNode* pNode, s2K& sBuf);

    // Returns true if a given node is a given type ** or is derived from that type **
    static b1 bIsOfType(PlanNode* pNode);

    static Stz* szSchema(QueryNode* pTableRef);
    static Stz* szTable(QueryNode* pTableRef);


  protected:
    static Stz* szTypeDefinition(eET eExprType);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

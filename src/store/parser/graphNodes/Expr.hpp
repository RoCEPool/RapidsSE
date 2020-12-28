/*============================================================================*/
/* Expr.hpp - a generic expression                                            */
/*============================================================================*/
#pragma once

namespace Parser
{
  class Expr : LogicalNode
  {
  public:
    // Expression Type
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
    static QueryNode* pCreateLiteral(eET eType, Value xVal);
    static QueryNode* pCreateColumnRef(Text* pColName);
    static void       vPrint(QueryNode* pStmt, s256& sBuf);


  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

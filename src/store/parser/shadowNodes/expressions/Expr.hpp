/*============================================================================*/
/* Expr.hpp - a generic expression                                            */
/*============================================================================*/
#pragma once

namespace Parser
{
  class Expr : public ShadowNode
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
    static void       vPrint(QueryNode* pStmt, s256& sBuf);

  protected:
    static Stz* szTypeDefinition(eET eExprType);

  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

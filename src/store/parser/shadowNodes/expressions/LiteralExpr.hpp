/*============================================================================*/
/* LiteralExpr.hpp - a literal expression                                     */
/*============================================================================*/
#pragma once

namespace Parser
{
  class LiteralExpr : public Expr
  {
  private:
    // Shalt never be instantiated. Static functions only.
    LiteralExpr();

  public:
    static QueryNode* pCreate(eET eType, Value xVal);

    static void       vPrint(QueryNode* pStmt, s256& sBuf);


  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

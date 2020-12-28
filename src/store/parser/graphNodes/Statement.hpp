/*============================================================================*/
/* Statement.hpp - identifies the type of query being planned                 */
/*============================================================================*/
#pragma once

namespace Parser
{
  class Statement : LogicalNode
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
    static void       vPrint(QueryNode* pStmt, s256& sBuf);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

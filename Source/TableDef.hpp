/*============================================================================*/
/* TableDef.hpp -- Table Definition                                           */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class TableDef : public Object
  {
  friend class Table;
  
  private:
    s32 m_sTableNm;          // table name
    eTT m_eTableType;        // table type
  
    StreamDef* m_pStreamDef; // definition
  
    s4K m_sCmd;              // the command which created this def

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    TableDef(void);
   ~TableDef(void) noexcept(false);
  
    static eCT eColumn(Stz* szType);
  
    eEC eParse(Stz* szCmd);
    
  private:

    eEC eParseColumn(TokenVec* pTknVec, i8 i_Col);

    eEC eParseColumnBoolean  (TokenVec* pTknVec, Column* pCol);
    eEC eParseColumnDate     (TokenVec* pTknVec, Column* pCol);
    eEC eParseColumnDecimal  (TokenVec* pTknVec, Column* pCol);
    eEC eParseColumnFloat    (TokenVec* pTknVec, Column* pCol);
    eEC eParseColumnInteger  (TokenVec* pTknVec, Column* pCol);
    eEC eParseColumnInterval (TokenVec* pTknVec, Column* pCol);
    eEC eParseColumnTime     (TokenVec* pTknVec, Column* pCol);
    eEC eParseColumnTimestamp(TokenVec* pTknVec, Column* pCol);
    eEC eParseColumnVarchar  (TokenVec* pTknVec, Column* pCol);

    eEC eParseColumnClauses  (TokenVec* pTknVec, Column* pCol);
    eEC eParseAsc            (TokenVec* pTknVec, Column* pCol);
    eEC eParseDesc           (TokenVec* pTknVec, Column* pCol);
    eEC eParseFixed          (TokenVec* pTknVec, Column* pCol);
    eEC eParseMax            (TokenVec* pTknVec, Column* pCol);
    eEC eParseMin            (TokenVec* pTknVec, Column* pCol);
    eEC eParseNotNull        (TokenVec* pTknVec, Column* pCol);
    eEC eParseRandom         (TokenVec* pTknVec, Column* pCol);

    eEC eParseTableClauses(TokenVec* pTknVec);
    eEC eParsePartitionBy(TokenVec* pTknVec);
    eEC eParseRowSize(TokenVec* pTknVec);
    
    Footer* aFooter(void);
  };
  
  TableDef* _pTableDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

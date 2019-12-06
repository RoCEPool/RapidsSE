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
  
    eEC eExtractBoolean  (Column* pCol, TokenVec* pTknVec);
    eEC eExtractDate     (Column* pCol, TokenVec* pTknVec);
    eEC eExtractDecimal  (Column* pCol, TokenVec* pTknVec);
    eEC eExtractFloat    (Column* pCol, TokenVec* pTknVec);
    eEC eExtractInteger  (Column* pCol, TokenVec* pTknVec);
    eEC eExtractInterval (Column* pCol, TokenVec* pTknVec);
    eEC eExtractTime     (Column* pCol, TokenVec* pTknVec);
    eEC eExtractTimestamp(Column* pCol, TokenVec* pTknVec);
    eEC eExtractVarchar  (Column* pCol, TokenVec* pTknVec);

    eEC eExtractPartitionBy(TokenVec* pTknVec);
    
    Footer* aFooter(void);
  };
  
  TableDef* _pTableDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

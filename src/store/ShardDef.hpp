/*============================================================================*/
/* ShardDef.hpp -- Shard Definition                                           */
/*============================================================================*/
#pragma once

#include "Definition.hpp"

namespace STORE
{
  class ShardDef : public Definition,
                   public WspConvergent_t<C_64KB,void,void>
  {
  public:
    s256 m_sShardDefNm;   // shardDef name
  
    Location m_oLocation; // logical location of stream def

    i8     m_nColumns;                    // total columns
    Column m_xaColumn[C_MAX_DEFCOLUMNS];  // array of columns
    i8     m_nFixedSized;                 // size of fixed size row

    RowPix  m_oRowPix;                    // row pix
  
  private:

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ShardDef(void);
    ShardDef(ShardDef* pShardDef);
   ~ShardDef(void) noexcept(false);
    
    static eCT eColumn(Stz* szType);
    static void vPostRecv(void* aShardDef);
    static void vRepair(Object* pObject, eRF eRepair);

    void    vAddColumn(Stz* sColumnNm, eCT eColumnType,
                       i8 iColumnLength, i8 iColumnScale);
    void    vAsString(String* pString);
    i8      nColumns(void);
    Column* pFindColumn(Stz* sColumnNm);
    Column* pGetColumn(i8 i_Col);
    void    vListContents(i8 i_Depth);
    Location* pLocation(void);
    void    vShardDefNm(Stz* szShardDefNm);
    Stz*    szShardDefNm(void);
    eEC     eParse(Stz* szCmd);
    void    vPrint(void);
    void    vResetPointers(void);
    RowPix* pRowPix(void);
    void    vToColPix(RowPix* pRowPix, bool bPtnHash);
    void    vToRowPix(RowPix* pRowPix, bool bPtnHash);
    void    vToMetadata(Stz* sMetaData);
    
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
    eEC eParseNotNull        (TokenVec* pTknVec, Column* pCol);
    
    eEC eParseTableClauses(TokenVec* pTknVec);
    eEC eParsePartitionBy(TokenVec* pTknVec);
    eEC eParseRowSize(TokenVec* pTknVec);
    
  public:
    Footer* aFooter(void);
  };
  
  ShardDef* _pShardDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* StreamDef.hpp -- Stream Definition                                         */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class StreamDef : public Definition
  {
  public:
    s32    m_sName;         // name
    s256   m_sPath;         // full name
    i8     m_iStreamId;     // unique stream identifier
    i8     m_nDefSets;      // eg. 3
    i8     m_nDefColumns[C_MAX_STREAMDEFS];
    i8     m_i_DefStart[C_MAX_STREAMDEFS];    // 0, 4, 9
    Column m_xaColumn[C_MAX_DEFCOLUMNS];
  
    i8     m_nFixedSized;   // is this a fixed size row?
    
    bool   m_bTemp;         // is this a temporary definition?
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StreamDef(void);
    StreamDef(StreamDef* pStreamDef);
   ~StreamDef(void) noexcept(false);
    
    static eCT eColumn(Stz* szType);
    
    void vAdd(StreamDef* pStreamDef);
    void vAddDefSet(void);
    void vAddColumn(Stz* ColumnNm, eCT eColumnType);
    void vAddColumn(Stz* sColumnNm, eCT eColumnType, i8 iColumnLength, i8 iColumnScale);
    void vAddColumns(StreamDef* pStreamDef);
    void vAsString(String* pString);
    i8 nColumns(i8 iSubSet);
    i8 iFindColumn(char* sColumnNm, i8& iColVec);
    Column* pGetColumn(i8 iColumn);
    void vName(Stz* sName);
    Stz* szName(void);
    eEC eParse(Stz* szCmd);
    void vPrint(void);
    void vToColPix(ColPix* axColPix, bool bPtnHash);
    void vToStreamPix(StreamPix* aStreamPix, bool bPtnHash);
    void vToMetadata(Stz* sMetaData);
    
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
  
  StreamDef* _pStreamDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

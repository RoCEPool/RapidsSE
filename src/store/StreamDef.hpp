/*============================================================================*/
/* StreamDef.hpp -- Stream Definition                                         */
/*============================================================================*/
#pragma once

#include "Definition.hpp"

namespace STORE
{
  class StreamDef : public Definition,
                    public WspConvergent_t<C_64KB,void,void>
  {
  public:
    s256   m_sName;                       // streamDef name
    i8     m_iStreamId;                   // unique stream identifier
    i8     m_nColumns;                    // total columns
    Column m_xaColumn[C_MAX_DEFCOLUMNS];  // array of columns
    i8     m_nFixedSized;                 // size of fixed size row

    RowPix  m_oRowPix;                    // row pix
  
  private:
    SetPix* m_pSetPix;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StreamDef(void);
    StreamDef(StreamDef* pStreamDef);
   ~StreamDef(void) noexcept(false);
    
    static eCT eColumn(Stz* szType);

    void    vAddColumn(Stz* sColumnNm, eCT eColumnType, i8 iColumnLength, i8 iColumnScale);
    void    vAsString(String* pString);
    i8      nColumns(void);
    Column* pFindColumn(Stz* sColumnNm);
    Column* pGetColumn(i8 i_Col);
    void    vName(Stz* sName);
    Stz*    szName(void);
    eEC     eParse(Stz* szCmd);
    void    vPrint(void);
    RowPix* aRowPix(void);
    SetPix* pSetPix(void);
    void    vSetPix(StreamDef* pStreamDef);
    void    vToColPix(ColPix* axColPix, bool bPtnHash);
    void    vToRowPix(RowPix* aRowPix, bool bPtnHash);
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
    
    Footer* aFooter(void);
  };
  
  StreamDef* _pStreamDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

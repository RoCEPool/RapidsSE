/*============================================================================*/
/* StreamDefBase.hpp -- Stream Definition Base                                */
/*============================================================================*/
#pragma once

#include "Definition.hpp"

namespace STORE
{
  class StreamDefBase : public Definition,
                        public WspConvergent_t<C_64KB,void,void>
  {
  public:
    s256 m_sStreamDefNm; // streamDef name
    
    bool m_bReplicated;  // replicated stream/table?
  
    Location m_oLocation; // logical location of stream def

    i8     m_nColumns;                    // total columns
    Column m_xaColumn[C_MAX_DEFCOLUMNS];  // array of columns
    i8     m_nFixedSized;                 // size of fixed size row

    RowPix  m_oRowPix;                    // row pix
    
  public:
    StreamDefBase(eCN eClassNo);
    StreamDefBase(eCN eClassNo, StreamDefBase* pStreamDefBase);

   ~StreamDefBase(void) noexcept(false);

    static void vRepair(Object* pObject);

    static eCT eColumn(Stz* szType);
    static void vPostRecv(void* aStreamDef);

    void    vAddColumn(Stz* sColumnNm, eCT eColumnType,
                       i8 iColumnLength, i8 iColumnScale);
    void    vAppendStreamDef(StreamDefBase* pStreamDef);
    void    vAppendStreamDef(StreamDefBase* pStreamDef, Stz* sColPrefix);
    void    vAsString(String* pString);
    i8      nColumns(void);
    Column* pFindColumn(Stz* sColumnNm);
    Column* pGetColumn(i8 i_Col);
    void    vListContents(i8 i_Depth);
    Location* pLocation(void);
    eEC     eParseCmd(Stz* szCmd);
    void    vPrint(void);
    bool    bReplicated(void);
    void    vReplicated(bool bReplicated);
    void    vResetPointers(void);
    RowPix* pRowPix(void);
    void    vStreamDefNm(Stz* szStreamDefNm);
    Stz*    szStreamDefNm(void);
    void    vToColPix(RowPix* pRowPix, bool bPtnHash);
    void    vToRowPix(RowPix* pRowPix, bool bPtnHash);
    void    vToMetadata(Stz* sMetaData);
    
  private:
    
    eEC eParse(Stz* szCmd);
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
    
    eEC eParseTableClauses   (TokenVec* pTknVec);
    eEC eParsePartitionBy    (TokenVec* pTknVec);
    eEC eParseReplicated     (TokenVec* pTknVec);
    eEC eParseRowSize        (TokenVec* pTknVec);

  };

  StreamDefBase* _pStreamDefBase(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

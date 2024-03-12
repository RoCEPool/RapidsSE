/*============================================================================*/
/* StreamDefBase.hpp -- Stream Definition Base                                */
/*============================================================================*/
#pragma once

#include "Definition.hpp"

#include "eSD.h"


namespace STORE
{
  class StreamDefBase : public Definition,
                        public WspConvergent_t<C_64KB,void,void>
  {
  private:
		s2K m_sStreamDefNm;  // streamDef name
    
    bool m_bReplicated;   // replicated stream/table?
  
    Location m_oLocation; // logical location of stream def

    i8     m_nColumns;    // total columns
    Column m_xaColumn[C_MAX_COLUMNS];  // array of columns

    RowPix m_oRowPix;     // row pix
    i8     m_jRowSize;    // size of fixed size row
    i8     m_jTableSize;  // estimate of table size

    i8     m_nOrders;     // number of orderBy columns
    b8     m_bUnique;     // is the stream unnique
    i8     m_iaOrder[C_MAX_STREAM_ORDER]; // array of stream order columns
    eSD    m_eaDirection[C_MAX_STREAM_ORDER]; // array of sort direction

  public:
    StreamDefBase(eCN eClassNo);
    StreamDefBase(eCN eClassNo, StreamDefBase* pStreamDef_arg);

   ~StreamDefBase(void) noexcept(false);

    static void vRepair(Object* pObject);

    static eCT eColumn(Stz* szType);
    static eDM eDatum(Stz* szType);
    static void vPostRecv(void* aStreamDef);

    void    vAddColumn(Stz* sColumnNm,
                       TypeId xTypeId,
                       i8 iColumnLength,
                       i8 iColumnScale,
                       bool bColumnNullable);
    void    vAddOrder(i8 i_Column, eSD eDirection=SD_ASC);
    void    vAppendStreamDef(StreamDefBase* pStreamDef);
    void    vAppendStreamDef(StreamDefBase* pStreamDef, Stz* sColPrefix);
    void    vApplyOrder(StreamDefBase* pStreamDef_apply);
    void    vAsString(String* pString);
  
    i8      iColumnOrder(i8 i_Col);
    i8      nColumns(void);
 
    eSD     eDirection(i8 i_Order);
    void    vDirection(i8 i_Order, eSD eDirection);

    void    vDump(i8 i_Tab=0);

    Column* pFindColumn(Stz* sColumnNm);

    Column* pGetColumn(i8 i_Col);
    Column* pGetOrder(i8 i_Order);
  
    bool    bIsDuplicateName(Column* pColumn);
    bool    bIsOrder(i8 i_Col);

    void    vListContents(i8 i_Depth);
    Location* pLocation(void);
  
    i8      nOrders(void);
  
    eEC     eParseCmd(Stz* szCmd, String* psErrStr);
    void    vPrint(void);
  
    bool    bReplicated(void);
    void    vReplicated(bool bReplicated);
    void    vResetPointers(void);
    RowPix* pRowPix(void);
    i8      jRowSizeBytes(void);
  
    void    vStreamDefNm(Stz* szStreamDefNm);
    Stz*    szStreamDefNm(void);
  
    i8      jTableSize(void);
    void    vToColPix(RowPix* pRowPix, bool bAddKeyHash);
    void    vToRowPix(RowPix* pRowPix, bool bAddKeyHash);
    void    vToMetadata(String* psMetadata);

    bool    bUnique(void);
    void    vUnique(bool bUnique);

  private:
    
    eEC eParse(Stz* szCmd, String* psErrStr);
    eEC eParseColumn(TokenVec* pTknVec, i8 i_Col, String* psErrStr);
    
    eEC eParseColumnBoolean  (TokenVec* pTknVec, Column* pCol, String* psErrStr);
    eEC eParseColumnDate     (TokenVec* pTknVec, Column* pCol, String* psErrStr);
    eEC eParseColumnDecimal  (TokenVec* pTknVec, Column* pCol, String* psErrStr);
    eEC eParseColumnFloat    (TokenVec* pTknVec, Column* pCol, String* psErrStr);
    eEC eParseColumnInteger  (TokenVec* pTknVec, Column* pCol, String* psErrStr);
    eEC eParseColumnInterval (TokenVec* pTknVec, Column* pCol, String* psErrStr);
    eEC eParseColumnTime     (TokenVec* pTknVec, Column* pCol, String* psErrStr);
    eEC eParseColumnTimestamp(TokenVec* pTknVec, Column* pCol, String* psErrStr);
    eEC eParseColumnVarchar  (TokenVec* pTknVec, Column* pCol, String* psErrStr);
    
    eEC eParseColumnClauses  (TokenVec* pTknVec, Column* pCol, String* psErrStr);
    eEC eParseNotNull        (TokenVec* pTknVec, Column* pCol, String* psErrStr);
    
    eEC eParseTableClauses   (TokenVec* pTknVec, String* psErrStr);
    eEC eParsePartitionBy    (TokenVec* pTknVec, String* psErrStr);
    eEC eParseReplicated     (TokenVec* pTknVec, String* psErrStr);
    eEC eParseRowSize        (TokenVec* pTknVec, String* psErrStr);
    eEC eParseTableSize      (TokenVec* pTknVec, String* psErrStr);

		eEC eValidateColumnName   (TokenVec* pTknVec, String* psErrStr);
    eEC eValidateTableName   (TokenVec* pTknVec, String* psErrStr);

  };

  StreamDefBase* _pStreamDefBase(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

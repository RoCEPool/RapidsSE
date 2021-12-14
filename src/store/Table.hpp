/*============================================================================*/
/* Table.hpp - Table                                                          */
/*============================================================================*/
#pragma once

//#include <vector>
#include "Index.hpp"
#include "IndexTree.hpp"
#include "eTT.h"
#include "Parcel.hpp"
#include "TablePtn.hpp"

namespace STORE { class InfoSet; class StreamDef; class ShardDef; }

namespace STORE
{
  class Table : public Operand
  {
  private:
    s32 m_sTableNm; // table name (leave for the moment)
    eTT m_eKind;    // table kind
  
    Location   m_oLocation;   // logical location of table
    StreamDef* m_pStreamDef;  // table stream def
    ShardDef*  m_pShardDef;   // partition/sharding key def

    BucketMap_dyn* m_pBucketMap;  // bucket (to partition) map

    IdxVec*    m_pIdxVec;     // index vector
    
    TablePtnVec*  m_pTablePtnVec; // table partition vector

    Parcel* m_pParcel;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Table* pTable);
    void  operator delete(void*);
    
    Table(Stz* szTableNm, eTT eKind);
   ~Table(void) noexcept(false);
    
    static Stz*       szCurrTable(DicStack* pDicStack);
    static void       vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Table*     pFindTable(DicStack* pDicStack, Stz* aTableNm);
    static IndexDef*  pIndexDef(DicStack* pDicStack, Stz* aTableNm, i8 iIndexNo);
    static StreamDef* pStreamDef(DicStack* pDicStack, Stz* aTableNm);
  
    static void       vPostRecv(void* aTable);
    
    i8      iPtnNoToTablePtn(PtnId tPtnId);
    PtnId   tTablePtnToPtnId(i8 i_TablePtn);

    void    vAddIndex(Index* pIndex);
    void    vAnalyze(i8 i_TablePtn, i8& nBuffers, i8& nRows);
    void    vAppendBuffer_replicated(DataBuffer* pDataBuffer);

    BucketMap_dyn *pBucketMap(void);
    void vBucketMap(BucketMap_dyn *pBucketMap);

    i8      nBuffers(void);
    void    vCompactIndex(Index* pIndex);
    void    vCreatePtnDef(void);
    void    vContentDump(void);
    Column* pFindColumn(Stz* szColNm);
    Index*  pFindIndex(Stz* szIndexNm);
    IdxVec* pIdxVec(void);
    i8      nIndexBuffers(i8 i_Index);
    bool    bIsDefinitionOnly(void);
    bool    bIsInternallyDefined(void);
    bool    bIsReplicated(void);
    bool    bIsUserCreated(void);
    eTT     eKind(void);
    void    vKind(eTT eKind);
    void    vListContents(i8 i_Depth);
    bool    bPopulateIndex(i8 iIndexNo);
    void    vRemoveIndex(Index* pIndex);
    void    vResetPointers(void);
    EntBuffer* pRootBuffer(i8 i_TablePtn, i8 i_Index);
    ShardDef*  pShardDef(void);
    StreamDef* pStreamDef(void);
    void       vStreamDef(StreamDef* pStreamDef);
    Stz*    szTableNm(void);
    TablePtn* pTablePtn(i8 i_TablePtn);

    PtnId   tBktToLglPtn(i8 iBktNo);

    i8      iPtnToBktNext(PtnId tPtnId, i8 iBktNo);

    
    //void vInsdelBuffer(i8 iTxId, i8 i_TablePtn, CrwBuffer* pCrwBuffer);
    //void vInsdelBuffer(i8 iTxId, i8 i_TablePtn, RowBuffer* pRowBuffer);
    //void vUpsertBuffer(i8 iTxId, i8 i_TablePtn, CrwBuffer* pCrwBuffer);
    bool bVerifyTable(void);
    bool bVerifyIndexes(void);
    bool bVerifyIndex(Index* pIndex);


    i8   iPrepareParcel(void);
    void vPrepareBktMap(void);

    Parcel* pParcel(void);
    void vDumpLocation(void);

    i8   nParcelObjects(void);
    Object* pGetParcelObject(i8 iIdx);

    i8   iBuildParcelObjs(Object* pObj);
    i8   iBuildParcel(Parcel* pParcel);
    i8   iReassembleObj(void);
    i8   iReassembleFromParcel(Parcel* pParcel);
    bool bUpdateBktMap(Parcel* pParcel);
    Stz* szDumpTablePtn(void);
    Stz* szDumpBucketMap(void);

  private:
    Footer* aFooter(void);
  };
  
  Table* _pTable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

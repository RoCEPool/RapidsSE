/*============================================================================*/
/* Pds.hpp -- Primary Data Structure (PDS)                                    */
/*============================================================================*/
#pragma once

#include "eBT.h"
#include "SkipListNode.hpp"
#include "SkipListBlock.hpp"

namespace BASE
{
  typedef i8 (*CompareFunc)(void* pKey_l, void* pKey_r);

  class Pds : public DataBuffer
  {
    static const i8 C_WSP_INTERNALHEAP = C_SIZE_LARGE_OBJECT - C_10KB;
    static const i8 C_MIN_KEY_LEN = sizeof(i8);

  public:
    i8 m_nBlockSize; // Size of SLN + Item in bytes
    i8 m_nPaddingPerBlk; // Size of padding for each Skip List Block
    u2 m_uFreeListRootOffset;
    i8 m_iFreeListEntries; // Number of free entries available (Does not include dummy root node)

    CompareFunc m_fpCompare;  // compare function

    u2 m_uInUseRootOffset;
    i8 m_iNumEntries[C_MAX_LVL_LEVELS];

    eBT     m_eBufferType;

    RowPix m_oRowPix;

    // ---------------------------
    i8    m_iSynthRowKey;     // Synthetic key for rows added to Pds

    // temporary variables
    Row* m_aRowCur;     // current dst row
    i8   m_i_RowCrs;    // current logical row number
    i8   m_i_ColCrs;    // current working column
    void* m_aStzSet;     // current pointer to string set area
    i8   m_iCurStzSetSpace; // current space left for string set area

    // Save the last usable offset for asserts
    PdsConvergentHeap_t<C_WSP_INTERNALHEAP,SkipListBlock,Stz> m_xWsp;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Pds(RowPix* pRowPix, eBT eBufferType = BT_DATA);                          // When Pds is used to store rows
    Pds(StreamDef* pStreamDef, bool bAddKeyHash, eBT eBufferType = BT_DATA);  // When Pds is used to store rows
    Pds(i8 nBytes, CompareFunc fpCompare);                                    // When Pds is used to track BufferIds
    ~Pds(void) noexcept(false);

    static void* aGetCellArray(Pds* pPdsRowBuffer, i8 iRow);
    static void vRepair(Object* pObject);

    // public:
    Row*            pAllotRow(void);
    Row*            pAllotRowFull(char** aaStzSet);
    eBT             eBufferType(void);
    void            vBufferType(eBT eBufferType);
    Row*            pCopyRow(Row* pRow, bool bSrcHasPtnHash = false, bool bCopyPtnHash = false);
		Row*            pCopyRowArray(i8* pRowArray);
    void*           aAllotStzSet(void);
    void            vDecidedDelete(void);
    bool            bDelete(void* pKey);
    void            vDumpFreeList(void);
    void            vDumpInUseList(void);
    SkipListBlock*  pHeadSL(void);
    SkipListBlock*  pHeadFreeList(void);
    SkipListBlock*  pInsert(void* pKey);
    SkipListBlock*  pFind(void* pKey);
    SkipListBlock*  pFindLessThan(void* pKey);
    SkipListBlock*  pFindGreaterThan(void* pKey);
    SkipListBlock*  pFirstBlock(void);
    Row*            pFirstRow(void);
    i8              nFreeBytes(void);
    void            vInitPix(RowPix* pRowPix);
    void            vInitPix(StreamDef* pStreamDef, bool bAddKeyHash);
    i8              nItems(void);
    void*           aLastUsableByteAddr(void);
    i8              iLastUsableOffset(void);
    i8              iNumEntries(i8 iLevel);
    void            vPrintRows(File* pFilePtr, Stz* szLabel);
    void            vReset(void);
    RowPix*         pRowPix(void);
    void            vStzSetSize(i8 nBytes);
    i8              iStzSetSize(void);
    void            vValidate(void);
    bool            bVerify(void);

    // private:
    SkipListBlock*  aAllotSkipListBlock(void);
    void*           pGetAddrFromOffset(u2 uOffset);
    u2              uGetOffsetFromBase(void* aAddress);
    void            vMoveToFreeList(SkipListBlock* pSLBlock);
    SkipListBlock*  pNextSLBlock(SkipListBlock* pSLBlock , i8 iLevel);
    void            vPriorVecToKey(void* pKey, SkipListBlock* aPriorVec[]);
    bool            bSetNextSLBlock(SkipListBlock* pStartBlk, SkipListBlock* pEndBlock, i8 iLevel);
    bool            bSetNextSLBlock(SkipListBlock* pStartBlk, u2 uOffset, i8 iLevel);

    static i8     iIntCompare(void* aItem_l, void* aItem_r);

    // Row helper
    void      vAdvanceCursor(void);
    void      vDepositColVal(d8 dColVal);
    void      vDepositColVal(i8 iColVal);
    void      vDepositColVal(i8 i_Col, i8 iColVal);
    void      vDepositColVal(Stz* szColVal, i8 iMaxStzLen);
    String_d* azDepositString_begin(i8 nSize);
    void      vDepositString_update(i8 nTotalSize);
    void      vDepositString_end(i8 nSize);
    void      vDiscardInvalidRow(void);

  public:
    Footer* aFooter(void);
  };

  Pds* _pPds(void* aAddr);

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* EntBuffer.hpp -- Ent Buffer                                                */
/*============================================================================*/
#pragma once

#include "eDU.h"
#include "eIU.h"

//#define TRACE_COMPACT(x) x EndDefine
#define TRACE_COMPACT(x) EndDefine

//#define EB_TRACE_INSERT
//#define EB_LOCAL_VERIFY
//#define EB_SPLIT_RIGHT

#include "RowSingle.hpp"


#include "Ent_flat.hpp"

#include "EntWrk.hpp"

#define ELM0_OF_KEY(aKey)   ( *(i8*)aKey )

#define ELM0_OF_ENTRY(aEnt_int) ( aEnt_int->m_iIntElm0 )

#define END_TICKET(tTicket) ( C_ENT_END_TICKET == tTicket )

#define GET_ENTRY(tTicket)  ( &aItemArray[tTicket] )

#define NEXT_TICKET(aEnt_int,i_Lvl) ( aEnt_int->m_taNextEntry[i_Lvl] )

#define SCAN_IS_AT_PRIOR_POS(iKeyElm,iNextElm) ( iKeyElm <= iNextElm )

//#define SCAN_IS_AT_PRIOR_POS_DESC(iKeyElm,iNextElm) ( iKeyElm >= iNextElm )

#define STORE_PRIOR_POS(aVec,i_Lvl,aEnt) ( aVec[i_Lvl] = (Ent*)aEnt )

#define NEXT_ENTRY(aNext,aCurrent,i_Level,pEntBuffer) \
{ \
  tET tNextEntry = aCurrent->m_taNextEntry[i_Level]; \
  if (C_ENT_END_TICKET == tNextEntry) { \
    aNext = nullptr; \
  } \
  else { \
    i8 iNextEntryOffset = (i8)tNextEntry * C_ENT_QUANTA; \
    aNext = (Ent*) ((char*)pEntBuffer + 624 + iNextEntryOffset); \
  } \
}

#define NEXT_BUFFER(aNextBuffer,pEntBuffer) \
{ \
  if (C_UNKNOWN == pEntBuffer->m_tNextBuffer.m_iBufferNo) { \
    aNextBuffer = nullptr; \
  } \
  else { \
    aNextBuffer = _pEntBuffer(BufferNo::pToDataBuffer(pEntBuffer->m_tNextBuffer.m_iBufferNo)); \
  } \
}


namespace STORE
{
  static const bool C_NOT_FINAL    = false;
  static const bool C_FINAL        = true;
  
  class EntBuffer : public DataBuffer,
                    public MiniHeap_t<Ent>
  {
  friend class IndexTree;

  public:
    BufferNo m_tNextBuffer;   // next buffer
    BufferNo m_tPrevBuffer;   // previous buffer

    BufferNo m_tHigherBuffer; // higher buffer
    BufferNo m_tLowerBuffer;  // lower buffer

  protected:
    eKE        m_eEntKind;          // what kind of entries are in this buffer?
    char       m_cSpaceForTx[256];  // reserving space for tx/influx entries

    IndexTree* m_pIndexTree;        // indexDef this buffer belongs to
  
    SkipRoots  m_xSkipRoots; // skip roots
    Ent*       m_aLazyChain; // first lazy entry

		i8  m_nLevels;              // no of levels
    i8  m_nInsert;              // insert into this buffer
    i8  m_nSplit;               // splits of this buffer
    i8  m_nMove;                // moves into of this buffer
    i8  m_nDelete;              // delete into this buffer
  
    i8  m_nInsertEntUpwards;    // number of active aInsertEntUpwards()
    i8  m_nNewInsertIntoSplit;  // number of active pNewInsertIntoSplit()
    i8  m_nNewInsertIntoRight;  // number of active pNewInsertIntoRight()
 
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    EntBuffer(IndexTree* pIndexTree);
   ~EntBuffer(void) noexcept(false);

    static void vCopyLeftH0(EntBuffer** ppLeftH0, EntBuffer* pRightH0);
    static void vCopyReInitH1(EntBuffer* pH1, EntBuffer* pLeftH0);
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Ent* aGetNextEntry(EntBuffer** ppEntBuffer, Ent* aPrevEntry);
    static EntBuffer* pRefdEntBuffer(Ent* pItem);
    static EntBuffer* pRefdEntBuffer(Rrf& RowRef);
  
    static i8  iToEntryOffset(tET tEntryTicket);
    static tET tToEntryTicket(i8 iEntryOffset);

    void    vDropWrite(void);
    void    vGrabWrite(void);
    eKE     eEntKind(void);
    void    vEntKind(eKE eEntKind);

    Index*  pIndex(void);
    IndexDef*  pIndexDef(void);
    void    vPrintRefdRows(void);
  
    bool    bTreeDebug(void);
    i8      nTreeDeleteCnt(void);
    i8      nTreeInsertCnt(void);


    ////////////////////////////////////////////////////////////////////////////
    // buffer functions
  
    void       vBufferCount(i8& nBufferCount);
    EntBuffer* pHigherEntBuffer(void);
    void       vHigherEntBuffer(EntBuffer* pHigherBuffer);
		void       vNextEntBuffer(EntBuffer* pNextBuffer);
		EntBuffer* pPrevEntBuffer(void);
    void       vPrevEntBuffer(EntBuffer* pPrevBuffer);

    //SrtBuffer* pToSrtBuffer(i8 iKeyCol);
    
    ////////////////////////////////////////////////////////////////////////////
    // entry functions
  
    Ent* aEntArray(void);
    Ent* aLastEntry(void);
    Ent* aNextEntry(Ent* aPrevEntry);
    void vShowEntry(Ent* pEntry);
    SkipRoots* aSkipRoots(void);

    tET  tToTicket(Ent* pEntry);
  
  private:
    Ent* aAllotEntry(i8 nSize);
    
    ////////////////////////////////////////////////////////////////////////////
  
  public:
    void vAssertEntBuffer(void);
    bool bValidEntBuffer(void);
    
    ////////////////////////////////////////////////////////////////////////////
    // static move functions
    
    static Ent* pMoveToGreaterThanEntry(Ent* aEnt, Key* aGivenKey,
                                        EntBuffer** ppEntBuffer);
    static Ent* pMoveToGreaterThanEntry_gen(Ent* aEnt, Key* aGivenKey,
                                        EntBuffer** ppEntBuffer);
    static Ent* pMoveToGreaterThanEntry_int(Ent* aEnt, Key* aGivenKey,
                                        EntBuffer** ppEntBuffer);
    static Ent* pMoveToGreaterEqualEntry(Ent* aEnt, Key* aGivenKey,
                                         EntBuffer** ppEntBuffer);
    static Ent* pMoveToGreaterEqualEntry_gen(Ent* aEnt, Key* aGivenKey,
                                         EntBuffer** ppEntBuffer);
    static Ent* pMoveToGreaterEqualEntry_int(Ent* aEnt, Key* aGivenKey,
                                         EntBuffer** ppEntBuffer);
    static Ent* pMoveToNextEntry(Ent* aEnt,
                                 EntBuffer** ppEntBuffer);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // key positioning functions
    
    Ent*       aFindEnt_int(Key* aWithKey);
  
  
    EntBuffer* pFindLeafBuffer(Key* aForKey);
    EntBuffer* pFindLeafBufferAsc(Key* aForKey);
    EntBuffer* pFindLeafBufferAscDsc(Key* aForKey);

    EntBuffer* pFindLeafBuffer_gen(Key* aForKey);
    EntBuffer* pFindLeafBuffer_ascGen(Key* aForKey);
    EntBuffer* pFindLeafBuffer_ascDscGen(Key* aForKey);

    EntBuffer* pFindLeafBuffer_int(Key* aForKey);
    EntBuffer* pFindLeafBuffer_ascInt(Key* aForKey);
    EntBuffer* pFindLeafBuffer_ascDscInt(Key* aForKey);

    Ent*       aFindEntryBefore(Key* aGivenKey);

    Ent*       aFindEntryBefore_gen(Key* aGivenKey);
    Ent*       aFindEntryBefore_ascGen(Key* aGivenKey);
    Ent*       aFindEntryBefore_ascDscGen(Key* aGivenKey);

    Ent*       aFindEntryBefore_int(Key* aGivenKey);

    Ent*       pFindEntryPrevTo_gen(Ent* aGivenEnt, Key* aWithKey);
    Ent*       pFindEntryPrevTo_ascGen(Ent* aGivenEnt, Key* aWithKey);
    Ent*       pFindEntryPrevTo_ascDscGen(Ent* aGivenEnt, Key* aWithKey);

    Ent*       pFindEntryPrevTo_int(Ent* aGivenEnt, Key* aWithKey);

    void       vPosVecPriorToBuffer(EntBuffer* pToRefdBuffer,
                                    Key* aWithKey,
                                    Ent* aPriorVec[]);
    void       vPosVecPriorToBuffer_gen(EntBuffer* pToRefdBuffer,
                                        Key* aWithKey,
                                        Ent* aPriorVec[]);
    void       vPosVecPriorToBuffer_int(EntBuffer* pToRefdBuffer,
                                        Key* aWithKey,
                                        Ent* aPriorVec[]);

  
    void       vPosVecPriorToEntry(Ent* aToEntry,
                                   Key* aWithKey,
                                   Ent* aPriorVec[]);
    void       vPosVecPriorToEntry_gen(Ent* aToEntry,
                                       Key* aWithKey,
                                       Ent* aPriorVec[]);
    void       vPosVecPriorToEntry_int(Ent* aToEntry,
                                       Key* aWithKey,
                                       Ent* aPriorVec[]);

    void       vPosVecPriorToEntry_int(Key* aWithKey,
                                       Ent* aPriorVec[]);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // complex functions
  
    //void        vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries);
    void        vAnalyze(i8& iBuffers, i8& iEntries);
    void        vBuildSkipListSorted(void);
    void        vDeleteEntry(Ent** aaEnt);
    void        vDeleteUpwards(eDU eReason, Ent* aDeleteEntry, i8 iRefdBufferNo,
                               EntBuffer** ppRootBuffer);
    void        vDump(bool bEntries = true);
    EntBuffer*  pFirstChildBuffer(void);
    Ent*        aInsertEntAbs(Ent* aEntry);
    Ent*        aInsertEntAbsL0(Ent* aEntry, Ent** aaPriorEntry);
    static Ent* aInsertEntTree(Ent* aEntry, EntBuffer** ppRootBuffer);
    void        vLazyBuild(void);
    i8          iLengthList(void);
    i8          iLengthSubList(void);
    Ent*        aNextEntry_flat(Ent* aEnt, i8 i_Offset);
    void        vPurgeUpwards(Ent* aDeleteEntry, EntBuffer** ppRootBuffer);
    void        vReset(void);
    void        vRestore(void);
    void        vRestoreIndex(Index* pIndex);
    void        vShow(bool bEntries = false, Ent* aMarkEntry = nullptr);
    void        vShowShort(void);
    void        vShowSubList(void);
    void        vShowTree(bool bDumpBuffer=false);
    i8          nSkipLength(i8 i_Level);
    void        vSkipLengths(i8& iL0Length_, i8& iL1Length_,
                             i8& iL2Length_, i8& iL3Length_,
                             i8& iL4Length_, i8& iL5Length_,
                             i8& iL6Length_);
    void        vSpliceAfter(EntBuffer* pSplicedBuffer);
    void        vTruncate(void);
    EntBuffer*  pUnlinkSubList(void);
    bool        bVerify(void);
  
    void        vNumberEntries(void);
    void        vRecordTickets(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    private:
    
    void       vBuildSkipList(void);
    void       vCompact(void);
    void       vCompactLeft(void);
    void       vCutEntry(Ent* aEnt, Ent* aPriorVec[]);
    void       vDipStick(void);
    void       vDumpPriorVec(Ent* aPriorVec[]);
    Ent*       aGetEntry(tET tEntryTicket);
    Ent*       aGetEntrySplit(i8 iSplitEntryNo, Ent** aaPriorEntry);
    void       vHandleFirstInsert(Ent* aInsertedKeyEntry,
                                  Ent* aOriginalFirstEntry,
                                  EntBuffer** ppRootBuffer);
    void       vHandleLeftInsert(Ent* aInsertedKeyEntry,
                                 Ent* aOriginalFirstEntry,
                                 EntBuffer** ppRootBuffer);
    EntBuffer* pInsertableBuffer(Ent* aEntry, EntBuffer** ppRootBuffer);
    Ent*       aInsertEntAbsLazy(Ent* aEntry);
    Ent*       aInsertEntBuffer(Ent* aEntry, EntBuffer** ppRootBuffer);
    Ent*       aInsertEntSkipList(Ent* aEntry);
    Ent*       aInsertEntUpwards(eIU eReason, Ent* aEntry, EntBuffer** ppRootBuffer);
    bool       bKeyBeforeBuffer(Key* aKey);
    FreeItem*  aMakeFreeEntry(Ent* aEnt);
    EntBuffer* pNewHigherBuffer(void);
    EntBuffer* pNewInsertBuffer(Ent* aEntry, EntBuffer** ppRootBuffer);
    bool       bNewInsertDecision(Ent* aEntry, EntBuffer** ppRootBuffer);
    EntBuffer* pNewInsertIntoRight(Ent* aEntry, EntBuffer** ppRootBuffer);
    EntBuffer* pNewInsertIntoSplit(Ent* aEntry, EntBuffer** ppRootBuffer);
    EntBuffer* pNewNextBuffer(void);
    Ent*       aPopLazyChain(void);

    Ent*       aSplitEntry(i8 iSplitRatio, Ent** aaPriorEntry);
    EntBuffer* pSplitBuffer(i8 iSplitRatio);
    
    void vTraceWhere(void);
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // verify functions
  
    bool bCheckPriorVec(Ent* aPriorVec[]);
  
    void vQuickVerify(Ent* aSeeEntry=nullptr);
    bool bQuickVerify(Ent* aNotSeeEntry);
  
    bool bVerifyH0Other(void);
    bool bVerifyLnAny(void);

    bool bVerifyAdjacent(void);
    bool bVerifyBufferRefs(void);
    bool bVerifyFirstKeys(void);
    bool bVerifyItems(void);
    bool bVerifyKeyBufferOrder(void);
    bool bVerifyKeyOrder(void);
    bool bVerifyNextPrev(void);
    bool bVerifyRowRefs(void);
    bool bVerifySkipListEntry(bool bShowKeys = false);
    bool bVerifySkipListLevels(bool bShowKeys = false);
    void vVerifyFault(const char* azFunc, const char* azFilePath, i8 iLineNo);

    public:
    Footer* aFooter(void);

	/*----------------------------------------------------------------------------*/
	/* get next entry                                                             */
	/*----------------------------------------------------------------------------*/
	#if 0

	inline Ent* aNextEntry(Ent* aCurrent, i8 iLevel)
	{
		_precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

		ASSERT(aCurrent);
		ASSERT(iLevel <= 5);

		ASSERT(aCurrent->bValidEnt());

		tET tNextEntry = aCurrent->tNextEntry(iLevel);

		if (C_ENT_END_TICKET == tNextEntry)
			return nullptr;

		//RASSERT(tNextEntry < m_nItems);

		//vTrace("DIFF: %lldB\n"_Stz,nDifference(this,aGetItem(0)));

		i8 iNextEntryOffset = iToEntryOffset(tNextEntry);
		Ent* aNextEnt = aGetItem(iNextEntryOffset);

		ASSERT(! aNextEnt || aNextEnt->bValidEnt());

		return aNextEnt;
	}

	/*----------------------------------------------------------------------------*/

	#else

		inline Ent* aNextEntry(Ent* aCurrent, i8 iLevel)
		{
			_precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);

			return aNextEntry_flat(aCurrent,iLevel);
		}

	#endif

	/*----------------------------------------------------------------------------*/
  /* get next buffer                                                            */
  /*----------------------------------------------------------------------------*/
	inline EntBuffer* pNextEntBuffer(void)
	{
		_precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

		return reinterpret_cast<EntBuffer*>(m_tNextBuffer.pGetDataBuffer());
	}

	/*----------------------------------------------------------------------------*/
	/* get lead entry (the entry after head entry                                 */
  /*----------------------------------------------------------------------------*/
	inline Ent* aLeadEntry(void)
	{
		_precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);


		Ent* aFirst = aNextEntry(aSkipRoots(),0);

		ASSERT(! aFirst || aFirst->bValidEnt());

		return aFirst;
	}
	};
	EntBuffer* _pEntBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

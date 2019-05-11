/*============================================================================*/
/* HashMap.cpp -- HashMap                                                     */
/*============================================================================*/

#include "Structures.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
HashMap* Structures::_pHashMap(void* aAddr)
{
  HashMap* pHashMap = reinterpret_cast<HashMap*>(aAddr);
  
  RASSERT_TYPE(pHashMap,CN_HASHMAP);
  
  return pHashMap;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* HashMap::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_HASHMAP);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* HashMap::operator new(size_t, HashMap* pHashMap)
{
  return pHashMap;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void HashMap::operator delete(void* pHashMapAddress)
{
  HashMap* pHashMap = reinterpret_cast<HashMap*>(pHashMapAddress);
  
  HeapMem::vFree(pHashMap);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HashMap::HashMap(bool bTrace, i8 nEntriesPerBuffer, i8 nArcsPerBuffer)
: Object(CN_HASHMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);
  
  m_bTrace            = bTrace;
  m_nEntriesPerBuffer = nEntriesPerBuffer ? nEntriesPerBuffer : C_HASH_ENTRIES;
  m_nArcsPerBuffer    = nArcsPerBuffer    ? nArcsPerBuffer    : C_HASH_ARCS;

  m_pRootBuffer     = new HashBuffer(this,nullptr,-1,false);
  m_pRootDuplicates = nullptr;

  m_nBuffers    = 0;
  m_iMaxDepth   = 0;
  m_nDupBuffers = 0;
  
  m_nEntries    = 0;
  m_nArcEntries = 0;
  m_nDupEntries = 0;
  
  _checkInitialised_(CN_HASHMAP);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HashMap::~HashMap(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP,_FOOTER_);
  
  delete m_pRootBuffer;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print analysis of this hash map                                            */
/*----------------------------------------------------------------------------*/
void HashMap::vAnalysis(bool bBufferAnalysis)
{
  if (bBufferAnalysis)
    m_pRootBuffer->vAnalysis();
  
  i8 iFullTree = (i8)pow(m_nArcsPerBuffer,m_iMaxDepth);
  
  vTrace("HM: (with %lld entries/buffer and %lld arcs/buffer)\n"
         "    Buffers %lld MaxDepth %lld "
         "manifested %lld%% of %lld buffers %lld entries\n"
         "    Entries %lld ArcEntries %lld µ%lld%% full\n"
         "    DupBuffers %lld DupEntries %lld\n"_Stz,
         
         m_nEntriesPerBuffer,
         m_nArcsPerBuffer,

         m_nBuffers,
         m_iMaxDepth,
         IntLib::iPercent(m_nBuffers,iFullTree),
         iFullTree,
         m_nEntriesPerBuffer * iFullTree,
         
         m_nEntries,
         m_nArcEntries,
         IntLib::iPercent(m_nEntries,m_nEntriesPerBuffer * m_nBuffers),
         
         m_nDupBuffers,
         m_nDupEntries);
}

/*----------------------------------------------------------------------------*/
/* append a key,value duplicate onto this hash map                            */
/*----------------------------------------------------------------------------*/
void HashMap::vAppendDuplicate(HashInsert* pHi, HashEntry* aOriginalEntry)
{
  if (! m_pRootDuplicates)
    m_pRootDuplicates = new HashBuffer(this,nullptr,C_UNKNOWN,true);
  
  if (! m_pRootDuplicates->bAppendDuplicate(pHi,aOriginalEntry)) {
    m_pRootDuplicates = new HashBuffer(this,m_pRootDuplicates,C_UNKNOWN,true);
    bool b = m_pRootDuplicates->bAppendDuplicate(pHi,aOriginalEntry);
    RASSERT(b);
  }
}

/*----------------------------------------------------------------------------*/
/* iterate each key,value in the hashmap                                      */
/*----------------------------------------------------------------------------*/
i8 HashMap::iEach(byte* aKey, i8 nBytes, i8& iCursor)
{
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* does given key exist in the hashmap                                        */
/*----------------------------------------------------------------------------*/
bool HashMap::bExists(byte* aKey, i8 nBytes)
{
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* does given key,value exist in the hashmap                                  */
/*----------------------------------------------------------------------------*/
bool HashMap::bExists(byte* aKey, i8 nBytes, i8 iValue)
{
 
  return false;
}

/*----------------------------------------------------------------------------*/
/* insert a key,value into this hashmap                                       */
/*----------------------------------------------------------------------------*/
void HashMap::vInsert(byte* yaKey, i8 nBytes, i8 iValue, bool bDebug)
{
  if (bDebug)
    vDebugTrap();
  if (341225 == iValue)
    vDebugPoint();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  HashInsert xHi;
  
  xHi.vSet(yaKey,nBytes,iValue);
  
  m_pRootBuffer->vInsertEntry(&xHi);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* HashMap::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_HASHMAP);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Entry.cpp -- buffer entry                                                  */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Entry::operator new(size_t)
{
  vFatal("fatal: Entry class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Entry::operator new(size_t, Entry* aEntry)
{
  return aEntry;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(eCN eClassNo, i8 nThisBytes, i8 nOffsets)
: Item(eClassNo, nThisBytes)
{
   m_xId.vOffsets(nOffsets);
  
  _checkInitialised_(eClassNo);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Entry::~Entry(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Row Data ----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get row address                                                            */
/*----------------------------------------------------------------------------*/
Row* Entry::aRowData(void)
{
  Row* aRow = reinterpret_cast<Row*>(aDataArea());
  
  return aRow;
}


/*----------------------------------------------------------------------------*/
/*-- Index -------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set entry using given key working entry                                    */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(Entry* pEntry, KeyWrk& xKeyWrk)
{
  RASSERT(this == pEntry);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  pEntry->vId(xKeyWrk.xId());
  pEntry->vOffsets(xKeyWrk.nOffsets());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  Key* aSrcKey = xKeyWrk.aKey();
  Key* aDstKey = pEntry->aKeyData();
  i8 nKeySize  = xKeyWrk.nKeySize();
  
  MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENTRY);
}

/*----------------------------------------------------------------------------*/
/* set entry using given row working entry                                    */
/*----------------------------------------------------------------------------*/
void Entry::vSetEntry(Entry* pEntry, RowWrk& xRowWrk)
{
  RASSERT(this == pEntry);
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  pEntry->vId(xRowWrk.xId());
  pEntry->vOffsets(xRowWrk.nOffsets());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy row into this entry
  
  Row* aSrcRow = xRowWrk.aRow();
  Row* aDstRow = pEntry->aRowData();
  i8 nRowSize  = xRowWrk.nRowSize();
 
  MemLib::vCopy(aDstRow,aSrcRow,nRowSize,CN_ENTRY);
}

/*----------------------------------------------------------------------------*/
/* show content                                                               */
/*----------------------------------------------------------------------------*/
void Entry::vShow(i8 iOffset, Index* pIndex, bool bShowKey)
{
  Key* aKey = this->aKeyData();
  
  vTrace("%lld: Entry: %p Size: %llu Offsets: %llu "_Stz,
         iOffset,
         this,
         nSizeItem(),
         nOffsets()
         );
  
  vTrace("Id: %3lli.%03lli "_Stz,
         m_xId.iBuffer(),
         m_xId.iItem()
         );
  
  //vTrace("Tx: %lld.%03lld "_Stz,
  //       Tx::iTxNode(m_iTxId),
  //       Tx::iTxNo(m_iTxId)
  //       );
  
  if (bShowKey && ! pIndex->bIsHidden())
    aKey->vShow(pIndex->pIdxColVec());
  
  if (pIndex->bIsHidden()) {
    //char* pRow = aRowData();
    Row* aThisRow = aRowData();
    vTrace("Row: "_Stz);
    i8 iRowSize = nSizeItem();
    if (iRowSize > 0) {
      aThisRow->vShow(pIndex->pTable()->pColumnVec(), true);
    }
  }
  
  
  vTrace(" offsets:"_Stz);
  for (i8 i_Offset = 0; i_Offset < nOffsets(); i_Offset++)
    vTrace(" %d"_Stz,aNEO()[i_Offset]);

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Row ---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Entry::vContentDump(void)
{
  vTrace("Entry %p: ThisBytes %lld %p\n"_Stz,
         this,
         //m_iTxId,
         nSizeItem(),
         aRowData()
         );
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

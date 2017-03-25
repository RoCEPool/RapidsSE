
/*============================================================================*/
/* Index.cpp - index portion of table partition                               */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
extern Arguments    gxArguments;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Index* DBobjects::_pIndex(void* pAddr)
{
  Index* pIndex = reinterpret_cast<Index*>(pAddr);
  
  RASSERT( !pIndex || CN_INDEX == pIndex->eClass() );
  
  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void Index::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_INDEX);
  ASSERT(pObject->eClass() == CN_INDEX);
  
  Data* pData = reinterpret_cast<Data*>(pObject);
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Index::operator new(size_t iSize)
{
  RASSERT(iSize);
  Index* pIndex = nullptr;
  
  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Index::operator new(size_t iSize, Index* pIndex)
{
  RASSERT(iSize);
  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Index::operator delete(void* pIndexAddress)
{
  RASSERT(pIndexAddress);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Index::Index(void)
: Object(CN_INDEX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  Word* pWord = Word::pToWord(this);
  
  m_iIndexNo = 0;
  m_sIndexName = pWord->sName();;
  m_eIndexType = IT_SKIP_LIST;
  m_pTable = nullptr;
  m_pIndexLevels = new IndexLevels();
  
  _checkInitialised_(CN_INDEX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Index::~Index(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set the table                                                              */
/*----------------------------------------------------------------------------*/
void Index::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);

  IndexBufferSeq* pIndexBufferSeq = m_pIndexLevels->pFirstBuffer();

  while (pIndexBufferSeq) {
    pIndexBufferSeq->vShow();
    
    Entry* pEntry = pIndexBufferSeq->pFirstEntry();
    
    while (pEntry) {
      pEntry->vContentDump();
      
      pEntry = pIndexBufferSeq->pNextEntry(pEntry);
    }
    pIndexBufferSeq = pIndexBufferSeq->pNextBuffer();
  }
}

/*----------------------------------------------------------------------------*/
/* populate the index                                                         */
/*----------------------------------------------------------------------------*/
void Index::vPopulate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  for (i8 iSlot = 0; iSlot < gxArguments.m_nSlots; iSlot++) {
    
    //i8 iPartition = Partition::iSlotToPartition(iSlot);
    
    RowStrand *pStrand = m_pTable->pPartition(iSlot)->pData()->pHeadStrand();
    
    i8 nBuffers = 0;
    i8 nStrands = 0;
    while (pStrand) {
      nStrands++;
      
        RowBuffer* pRowBuffer = pStrand->pFirstBuffer();
        i8 nRowsInBuffer = 0;
        i8 nRowBuffer = 0;
      
        while (pRowBuffer) {
          nRowBuffer++;
          nRowsInBuffer = pRowBuffer->nRows();
         
          Row* pRow = pRowBuffer->pFirstRow();
          bn nBufferNo = pRowBuffer->nBufferNo();
          i8 nRowNo = 0;
          
          while (pRow) {
            
            EntWsp xEntWsp;
            RowId xRowId;
            ValWsp xValWsp;
            KeyWsp xKeyWsp;
            i8 iKey;
            
            // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            // form the rowId
            xRowId.vBuffer(*nBufferNo);
            xRowId.vRow(nRowNo);
            
            // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            // set the rowId in the EntWsp
            xEntWsp.vRowId(xRowId);
            
            // get the first field (hardcoded)                            //ALKA
            StrVal* pStrVal = (StrVal*)pRow->pData();
            
            // extract the key from the first field
            iKey = pStrVal->iToI8();
            
            // add the key to the KeyWsp
            xValWsp = iKey;
            xKeyWsp.vAddElement(xValWsp);
            
            // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            // add key to entry
            xEntWsp.vAddKey(xKeyWsp);

            xEntWsp.vShow();
            
            // insert row entry in index
            m_pIndexLevels->vInsertEntry(xEntWsp);
            pRow = pRowBuffer->pNextRow(pRow);
            nRowNo++;
          }
          
          // move to the next rowBuffer
          pRowBuffer = pRowBuffer->pNextBuffer();
        }
      
      nBuffers += pStrand->nRowBuffers();
      pStrand = pStrand->pNextStrand();
    }
    
  }

}

/*----------------------------------------------------------------------------*/
/* set the table                                                              */
/*----------------------------------------------------------------------------*/
void Index::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* truncate the index                                                         */
/*----------------------------------------------------------------------------*/
void Index::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX,_FOOTER_);
  
  m_pIndexLevels->vTruncate();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Index::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEX);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


/*============================================================================*/
/* IndexData.cpp - index data portion of table partition                      */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
extern Grid*        gpGrid;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexData* DBobjects::_pIndexData(void* pAddr)
{
  IndexData* pIndexData = reinterpret_cast<IndexData*>(pAddr);
  
  RASSERT( !pIndexData || CN_INDEXDATA == pIndexData->eClass() );
  
  return pIndexData;
}

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void IndexData::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_INDEXDATA);
  ASSERT(pObject->eClass() == CN_INDEXDATA);
  
  Data* pData = reinterpret_cast<Data*>(pObject);
}

/*----------------------------------------------------------------------------*/
/* finalize                                                                   */
/*----------------------------------------------------------------------------*/
void IndexData::vFinalize(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_INDEXDATA);
  ASSERT(pObject->eClass() == CN_INDEXDATA);
  
  IndexData* pIndexData = reinterpret_cast<IndexData*>(pObject);
  ASSERT(pIndexData->bWordBody());
  
  pIndexData->vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IndexData::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_INDEXDATA);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IndexData::operator delete(void* pDataAddress)
{
  RASSERT(pDataAddress);
  IndexData* pIndexData = reinterpret_cast<IndexData*>(pDataAddress);
  
  Heap::vFree(pIndexData);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexData::IndexData(Index* pIndex)
: Object(CN_INDEXDATA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  m_pIndex = pIndex;
  m_pIndexLevels = new IndexLevels();
  
  _checkInitialised_(CN_INDEXDATA);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexData::~IndexData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
Str* IndexData::pAnalyze(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  i8 nRowsTotal = 0;
  i8 nBuffersTotal = 0;
  i8 nStrandsTotal = 0;
  s48K sAnalysis;
  
  sAnalysis.vCatSprint("N%lld: Analysis of Index %s of table %s\n"_Str,
                       gpGrid->iThisNode(),
                       m_pIndex->aIndexName()->s(),
                       m_pIndex->pTable()->aTableName());
  
  return sAnalysis;
}

/*----------------------------------------------------------------------------*/
/* dump the index                                                             */
/*----------------------------------------------------------------------------*/
void IndexData::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);

  /*
  IndexBuffer* pIndexBuffer = m_pIndexLevels->pFirstBuffer();

  while (pIndexBuffer) {
    pIndexBuffer->vShow();
    pIndexBuffer = pIndexBuffer->pNextBuffer();
  }
  */
  m_pIndexLevels->vDump();
  
}

/*----------------------------------------------------------------------------*/
/* get the index                                                              */
/*----------------------------------------------------------------------------*/
Index* IndexData::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the index levels                                                       */
/*----------------------------------------------------------------------------*/
IndexLevels* IndexData::pIndexLevels(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  return m_pIndexLevels;
}

/*----------------------------------------------------------------------------*/
/* get the index buffers used                                                 */
/*----------------------------------------------------------------------------*/
i8 IndexData::nIndexBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  i8 nBuffers = 0;
  
  IndexBuffer* pIndexBuffer = m_pIndexLevels->pFirstBuffer();
  
  while (pIndexBuffer) {
    nBuffers++;
    pIndexBuffer = pIndexBuffer->pNextBuffer();
  }
  
  return nBuffers;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for the row into the index                                 */
/*----------------------------------------------------------------------------*/
void IndexData::vInsertEntry(Row* pRow, i8 nRowNo, bn nBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  Id xId;
  ValWsp xValWsp;
  KeyWsp xKeyWsp;
  i8 iKey;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  xId.vBuffer(*nBufferNo);
  xId.vRow(nRowNo);
  
  // get the first field (hardcoded)                            //ALKA
  StrVal* pStrVal = (StrVal*)pRow->pData();
  
  // extract the key from the first field
  iKey = pStrVal->iToI8();
  
  // add the key to the KeyWsp
  xValWsp = iKey;
  xKeyWsp.vAddElement(xValWsp);
    
  // insert row entry in index
  m_pIndexLevels->vInsertEntry(xKeyWsp, xId, m_pIndex->pIdxColVec());
  
}

/*----------------------------------------------------------------------------*/
/* get next entry                                                             */
/*----------------------------------------------------------------------------*/
Entry* IndexData::pNextEntry(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  return m_pIndexLevels->pNextEntry(pEntry);
}

/*----------------------------------------------------------------------------*/
/* position the indexData at lower bound given key                            */
/*----------------------------------------------------------------------------*/
Entry* IndexData::aPosition(KeyWsp& xKeyWsp, RelOpAry* pOpAryLower)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  Key xKey(xKeyWsp);
  
  return m_pIndexLevels->aPositionL0(xKey, pOpAryLower, m_pIndex->pIdxColVec());
}

/*----------------------------------------------------------------------------*/
/* tidyup                                                                     */
/*----------------------------------------------------------------------------*/
void IndexData::vTidyUp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  delete m_pIndexLevels;
}

/*----------------------------------------------------------------------------*/
/* truncate the index                                                         */
/*----------------------------------------------------------------------------*/
void IndexData::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  m_pIndexLevels->vTruncate();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* IndexData::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

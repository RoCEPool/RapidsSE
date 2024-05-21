/*============================================================================*/
/* MetaDataTables.cpp -- MetaData Tables                                      */
/*============================================================================*/

#include "BASE.h"

#include "MetaDataTables.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,MetaDataTables,CN_METADATATABLES)
__aNewFunction(MetaDataTables,CN_METADATATABLES)
__vDeleteFunction(MetaDataTables,CN_METADATATABLES)
__aFooterFunction(MetaDataTables)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaDataTables::MetaDataTables(void)
    : Object(CN_METADATATABLES)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATATABLES,_FOOTER_);

  for (i8 i_Table = 0; i_Table < MD_ENUMS; i_Table++)
    m_apStreamDef[i_Table] = nullptr;

  _checkInitialised_(CN_METADATATABLES);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_METADATATABLES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaDataTables::~MetaDataTables(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_METADATATABLES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Delete table info from table of tables. Check TableType and exit if not allowed*/
/*----------------------------------------------------------------------------*/
bool MetaDataTables::bDeleteFromTableOfTables(Stz* szTableNm)
{
  _static_

  HashMap* pHashMap = AM::pHashMap();

  Table* pTable = AM::pMetaDataTableOfTables();

  TablePtn* pTablePtn = pTable->pTablePtn(0);

  BufferId oRootBufferId = pTablePtn->oRootBufferId();
  Pds* pRootPds = pTablePtn->pRootPds();

  PdsCursor* pCursor = new PdsCursor(pRootPds);

  // Iterate this table ptn root pds buffer that contains bufferId's of PdsRowBuffers

  while(! pCursor->bIsCurrNull()) {
    SkipListBlock* pSLBlock = pCursor->m_pCurrentBlock;

    BufferId oBufferId = pTablePtn->oExtractBufferId((Object*)pSLBlock->aItem());
    Pds* pPdsRowBuffer = _pPds(pHashMap->pValueAt(oBufferId.m_pvAsAddress, true));

    PdsCursor* pRowCursor = new PdsCursor(pPdsRowBuffer);

    // Iterate this PdsRowBuffer, searching for the rows which match this col value
    while(! pRowCursor->bIsCurrNull()) {
      Row* pRow = pRowCursor->pCurrRow();

      Stz* szRowTableNm = pRow->szColVal_ofs(2);
      eTT eTableType = (eTT)pRow->iColVal_int(3);

      if(szRowTableNm->bSame(szTableNm)) {

        // Ensure we are deleting a user defined table, otherwise stop and return false
        if(eTableType != TT_NORMAL) {
          pHashMap->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
          delete pRowCursor;
          delete pCursor;
          pHashMap->vDropKeyLock(oRootBufferId.m_pvAsAddress);
          return false;
        }

        i8* pKey = (i8*)pRowCursor->pCurrKey();

        // Found a match, delete this entry
        bool bResult = pPdsRowBuffer->bDelete(pKey);
        ASSERT(bResult);
        UNUSED(bResult);

        // If the pds row buffer is now empty, we can delete it
        if(pPdsRowBuffer->nItems() == 0) {
          void* pKey = pCursor->pCurrKey();
          pRootPds->bDelete(pKey);
          delete pPdsRowBuffer;
        } else {
          pHashMap->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
        }

        delete pRowCursor;
        delete pCursor;
        pHashMap->vDropKeyLock(oRootBufferId.m_pvAsAddress);

        return true;
      }
      pRowCursor->vNext();
    }

    delete pRowCursor;
    pHashMap->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);
  }
  delete pCursor;
  pHashMap->vDropKeyLock(oRootBufferId.m_pvAsAddress);

  return false;
}

/*----------------------------------------------------------------------------*/
/* Delete ALL rows that match this col value                                  */
/*----------------------------------------------------------------------------*/
void MetaDataTables::vDeleteAllRowsFromTable(Table *pTable, i8 iCol, Stz *szColVal)
{
  _static_

  HashMap* pHashMap = AM::pHashMap();

  TablePtn* pTablePtn = pTable->pTablePtn(0);

  BufferId oRootBufferId = pTablePtn->oRootBufferId();
  Pds* pRootPds = pTablePtn->pRootPds();

  PdsCursor* pCursor = new PdsCursor(pRootPds);

  // Iterate this table ptn root pds buffer that contains bufferId's of PdsRowBuffers

  while(! pCursor->bIsCurrNull()) {
    SkipListBlock* pSLBlock = pCursor->m_pCurrentBlock;

    BufferId oBufferId = pTablePtn->oExtractBufferId((Object*)pSLBlock->aItem());
    Pds* pPdsRowBuffer = _pPds(pHashMap->pValueAt(oBufferId.m_pvAsAddress, true));

    PdsCursor* pRowCursor = new PdsCursor(pPdsRowBuffer);

    // Iterate this PdsRowBuffer, searching for the rows which match this col value
    while(! pRowCursor->bIsCurrNull()) {
      Row* pRow = pRowCursor->pCurrRow();

      Stz* szRowTableNm = pRow->szColVal_ofs(iCol);

      if(szRowTableNm->bSame(szColVal)) {
        i8* pKey = (i8*)pRowCursor->pCurrKey();

        // Found a match. Shift the cursor to the next entry and delete this entry
        pRowCursor->vNext();
        bool bResult = pPdsRowBuffer->bDelete(pKey);
        ASSERT(bResult);
        UNUSED(bResult);
        continue;
      }
      pRowCursor->vNext();
    }

    // If the pds row buffer is now empty, we can delete it
    if(pPdsRowBuffer->nItems() == 0) {
      void* pKey = pCursor->pCurrKey();
      pCursor->vNext();
      pRootPds->bDelete(pKey);
      delete pPdsRowBuffer;
    } else {
      pHashMap->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);

      // Get the next PdsRowBuffer
      pCursor->vNext();
    }

    delete pRowCursor;
  }
  delete pCursor;
  pHashMap->vDropKeyLock(oRootBufferId.m_pvAsAddress);
}

Table* MetaDataTables::pTableFind(Stz *szTableNm)
{
  _static_

  HashMap* pHashMap = AM::pHashMap();

  Table* pTableOfTables = AM::pMetaDataTableOfTables();
  TablePtn* pTablePtn = pTableOfTables->pTablePtn(0);

  BufferId oRootBufferId = pTablePtn->oRootBufferId();
  Pds* pRootPds = pTablePtn->pRootPds();

  PdsCursor* pCursor = new PdsCursor(pRootPds);

  // Iterate this table ptn root pds buffer that contains bufferId's of PdsRowBuffers

  while(! pCursor->bIsCurrNull()) {
    SkipListBlock* pSLBlock = pCursor->m_pCurrentBlock;

    BufferId oBufferId = pTablePtn->oExtractBufferId((Object*)pSLBlock->aItem());
    Pds* pPdsRowBuffer = _pPds(pHashMap->pValueAt(oBufferId.m_pvAsAddress, true));

    PdsCursor* pRowCursor = new PdsCursor(pPdsRowBuffer);

    // Iterate this PdsRowBuffer, searching for the rows which match this col value
    while(! pRowCursor->bIsCurrNull()) {
      Row* pRow = pRowCursor->pCurrRow();

      Stz* szRowTableNm = pRow->szColVal_ofs(2);

      if(szRowTableNm->bSame(szTableNm)) {
        Table* pTable = (Table*)(pRow->iColVal_int(4));

        delete pRowCursor;
        pHashMap->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);

        delete pCursor;
        pHashMap->vDropKeyLock(oRootBufferId.m_pvAsAddress);

        return pTable;
      }
      pRowCursor->vNext();
    }

    pHashMap->vDropKeyLock(pPdsRowBuffer->oGetBufferId().m_pvAsAddress);

    // Get the next PdsRowBuffer
    pCursor->vNext();
    delete pRowCursor;
  }
  delete pCursor;
  pHashMap->vDropKeyLock(oRootBufferId.m_pvAsAddress);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get stream def for given metadata table                                    */
/*----------------------------------------------------------------------------*/
StreamDef* MetaDataTables::pStreamDef(eMD eTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATATABLES,_FOOTER_);

  return m_apStreamDef[eTable];
}

/*----------------------------------------------------------------------------*/
/* set stream def for given metadata table                                    */
/*----------------------------------------------------------------------------*/
void MetaDataTables::vStreamDef(eMD eTable, StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATATABLES,_FOOTER_);

  m_apStreamDef[eTable] = pStreamDef;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

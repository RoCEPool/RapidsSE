
/*============================================================================*/
/* PdsRowHelper.cpp -- Wrapper for Pds of Rows and string area                */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,PdsRowHelper,CN_PDSROWHELPER)
__aNewFunction(PdsRowHelper,CN_PDSROWHELPER)
__vDeleteFunction(PdsRowHelper,CN_PDSROWHELPER)
__aFooterFunction(PdsRowHelper)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PdsRowHelper::PdsRowHelper(void)
    : Object(CN_PDSROWHELPER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);

  m_iSynthKey  = 0;
  m_pRowPix  = nullptr;

  m_aRowCur  = nullptr;
  m_i_RowCrs = C_UNKNOWN;
  m_i_ColCrs = C_UNKNOWN;
  m_aStzSet  = nullptr;
  m_iCurStzSetSpace = 0;

  m_pPdsOfRows = nullptr;

  _checkInitialised_(CN_PDSROWHELPER);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PdsRowHelper::~PdsRowHelper(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);

  delete m_pPdsOfRows;
}


/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/

Row* PdsRowHelper::pAllotRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);

  RASSERT(m_pPdsOfRows);

  m_i_ColCrs  = 0;

  if (m_pPdsOfRows->nFreeBytes() < C_2KB)
    return nullptr;

  // Pass an integer key to Pds that we increment each time we insert
  // Essentially an append
  SkipListBlock* pSkipListBlock = m_pPdsOfRows->pInsert((void*) &(++m_iSynthKey));
  i8* pKey = (i8*)pSkipListBlock->aItem();
  *pKey = m_iSynthKey;

  // Calculate the address of the Row
  // <i8-key><Row>
  Row* aRowCur = aAdd<Row*>(pSkipListBlock->aItem(), sizeof(m_iSynthKey));

  // Initialise the row at the given addr
  new(aRowCur) Row();

  m_i_RowCrs++;

  return aRowCur;
}

/*----------------------------------------------------------------------------*/
/* allot a fixed size string set in this Pds from the tail(may return nulltpr)           */
/*----------------------------------------------------------------------------*/

void* PdsRowHelper::aAllotStzSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);

  RASSERT(m_pPdsOfRows);

  void* aStzSet = m_pPdsOfRows->aAllotStzSet();

  return aStzSet;
}

/*----------------------------------------------------------------------------*/
/* convert                                                                    */
/*----------------------------------------------------------------------------*/
void PdsRowHelper::vCopyRows(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);

  i8 nRows = pRowBuffer->nRows();
  if(nRows == 0)
    return;

  RowPix* pRowPix = pRowBuffer->pRowPix();
  if(pRowPix == nullptr)
    return;

  // Hard code Vch col length till issue is fixed
//  pRowPix->m_xaColPix[1].m_uLength = 10;
//  pRowPix->m_xaColPix[2].m_uLength = 32;
//  pRowPix->m_xaColPix[4].m_uLength = 32;

  i8 iRowSizeProper = Row::nRowProper(pRowPix->m_nColumns);

  if (m_pPdsOfRows == nullptr) {
    m_pPdsOfRows = new Pds(iRowSizeProper + sizeof(m_iSynthKey), nullptr, pRowPix);
  }

  RowPix* pPdsRowPix = &m_pPdsOfRows->m_oRowPix;
  RASSERT(pPdsRowPix->m_bInitialised);
  RASSERT(pPdsRowPix->m_nColumns);

  i8 nCols = pPdsRowPix->m_nColumns;

  // Total size of string area needed per row
  i8 iTotalStzArea = m_pPdsOfRows->iStzSetSize();

  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {

    Row* aRowSrc = pRowBuffer->pRowAddress(i_Row);

    // Allot a Row in the Pds
    m_aRowCur = pAllotRow();
    if (m_aRowCur == nullptr) {
      vTrace("%s: Unable to copy rows as the Pds is full\n"_Stz, __func__ );
      return;
    }

    RASSERT( 0 == m_i_ColCrs);

    // If this Row contains strings,
    // allot a String Set from the tail of the Pds
    if(iTotalStzArea) {
      m_aStzSet = aAllotStzSet();
      if (m_aStzSet == nullptr) {
        vTrace("%s: Unable to copy rows as the Pds string area is full\n"_Stz, __func__ );
        return;
      }
      m_iCurStzSetSpace = iTotalStzArea;
    }

    i8 iVal;
    Stz* szSrcStz;

    i8 i_Col;
    for (i_Col = 0; i_Col < nCols; i_Col++) {

      eDM eDatum = (eDM)pPdsRowPix->m_xaColPix[i_Col].m_uDatum;

      if (DM_VCH == eDatum) {
        // vch datum

        if (C_DATUM_NULL == aRowSrc->iColVal_int(i_Col)) {
          vDepositColVal(C_DATUM_NULL);
        }
        else {
          RASSERT(m_aStzSet != nullptr);
          szSrcStz = aRowSrc->szColVal_ofs(i_Col);
          vDepositColVal(szSrcStz, pPdsRowPix->m_xaColPix[i_Col].m_uLength);
        }
      }
      else {
        // all others

        iVal = aRowSrc->iColVal_int(i_Col);
        vDepositColVal(iVal);
      }
    }
  }

  // Reset the temporary variables
  vReset();
}

/*----------------------------------------------------------------------------*/
/* print rows                                                                 */
/*----------------------------------------------------------------------------*/
void PdsRowHelper::vPrintRows(File* pFilePtr, Stz* szLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  UNUSED(pFilePtr);

  RowPix* pPix = &m_pPdsOfRows->m_oRowPix;

  HV_Record hvRecord(pPix);

  PdsCursor* pCursor = new PdsCursor(m_pPdsOfRows);

  while( ! pCursor->bIsCurrNull()) {
    SkipListBlock* pSkipListBlock = pCursor->m_pCurrentBlock;
    Row* aRowCur = aAdd<Row*>(pSkipListBlock->aItem(), sizeof(m_iSynthKey));

    hvRecord->vToCsvRecord(aRowCur);
    hvRecord->vPrintAsRow(szLabel);
    pCursor->vNext();
  }
}

/*----------------------------------------------------------------------------*/
/* Reset the temporary variables                                              */
/*----------------------------------------------------------------------------*/
void PdsRowHelper::vReset(void)
{
  m_aRowCur  = nullptr;
  m_i_RowCrs = C_UNKNOWN;
  m_i_ColCrs = C_UNKNOWN;
  m_aStzSet  = nullptr;
  m_iCurStzSetSpace = 0;
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* advance cursor                                                           */
/*--------------------------------------------------------------------------*/
void PdsRowHelper::vAdvanceCursor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);

  m_i_ColCrs++;
}


/*--------------------------------------------------------------------------*/
/* deposit an d8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
void PdsRowHelper::vDepositColVal(d8 dColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);

  m_aRowCur->vColVal_flt(m_i_ColCrs,dColVal);

  vAdvanceCursor();

}

/*--------------------------------------------------------------------------*/
/* deposit an i8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
void PdsRowHelper::vDepositColVal(i8 iColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);

  m_aRowCur->vColVal_int(m_i_ColCrs,iColVal);

  vAdvanceCursor();
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
void PdsRowHelper::vDepositColVal(i8 i_Col, i8 iColVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);

  m_aRowCur->vColVal_int(i_Col,iColVal);
}

/*--------------------------------------------------------------------------*/
/* deposit an stz value into next cell                                      */
/*--------------------------------------------------------------------------*/
void PdsRowHelper::vDepositColVal(Stz* szColVal, i8 iMaxStzLen)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);

  RASSERT(m_iCurStzSetSpace > 0);

  String_d* azStringDyn = azDepositString_begin(iMaxStzLen);

  azStringDyn->vCopy(szColVal);
  i8 iTotalSize = azStringDyn->iTotalSize();
  RASSERT((m_iCurStzSetSpace - iTotalSize) >= 0);

  m_iCurStzSetSpace -= iTotalSize;

  // TODO: Arjun - Check with Ray: nLen or iMaxStzLen?
  vDepositString_end(iTotalSize);
}

/*--------------------------------------------------------------------------*/
/* begin deposit of string                                                  */
/*--------------------------------------------------------------------------*/
String_d* PdsRowHelper::azDepositString_begin(i8 nSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);

  String_d* szDeposited = new(m_aStzSet) String_d(nSize);

  // TODO: Arjun - Check type conversion with Ray
  i8 i_Ofs = nDifference(m_aRowCur->aColCell(m_i_ColCrs),szDeposited);

  m_aRowCur->vColVal_ofs(m_i_ColCrs,i_Ofs);

  vAdvanceCursor();


  return szDeposited;
}

/*--------------------------------------------------------------------------*/
/* take bytes for deposited string                                          */
/*--------------------------------------------------------------------------*/
void PdsRowHelper::vDepositString_end(i8 nSize)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSROWHELPER,_FOOTER_);

  m_aStzSet = aAdd<void*>(m_aStzSet, nSize);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
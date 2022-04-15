/*============================================================================*/
/* EachRowBuffer.cpp -- Each Row Buffer                                       */
/*============================================================================*/

#include "BASE.h"

#include "EachRowBuffer.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aNewFunction(EachRowBuffer,CN_EACHROWBUFFER)
__vDeleteFunction(EachRowBuffer,CN_EACHROWBUFFER)
__aFooterFunction(EachRowBuffer);

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
EachRowBuffer* STORE::_pEachRowBuffer(void* aAddress)
{
  EachRowBuffer* pEachRowBuffer = reinterpret_cast<EachRowBuffer*>(aAddress);
  
  return pEachRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EachRowBuffer::EachRowBuffer(void)
    : DataBuffer(CN_ROWBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EachRowBuffer::EachRowBuffer(eCN eClassNo)
    : DataBuffer(eClassNo, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EachRowBuffer::EachRowBuffer(eCN eClassNo, StreamDef* pStreamDef)
    : DataBuffer(eClassNo, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);

  vResolveRowPix(pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EachRowBuffer::EachRowBuffer(eCN eClassNo, RowPix* pRowPix)
    : DataBuffer(eClassNo, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);

  vCopyRowPix(pRowPix);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EachRowBuffer::~EachRowBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void EachRowBuffer::vPostRecv(void* aEachRowBuffer)
{
  _static_

  EachRowBuffer* pEachRowBuffer = _pEachRowBuffer(aEachRowBuffer);

  pEachRowBuffer->vRestorePointers();
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the row buffer that contains the given address                         */
/*----------------------------------------------------------------------------*/
EachRowBuffer* EachRowBuffer::pToEachRowBuffer(void* aAddr)
{
  _static_

  DataBuffer* pDataBuffer = pToDataBuffer(aAddr);

  EachRowBuffer* pEachRowBuffer =
      reinterpret_cast<EachRowBuffer*>(pDataBuffer);

  return pEachRowBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a new row                                                            */
/*----------------------------------------------------------------------------*/
Row* EachRowBuffer::pAllotRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  // allot a row
  Row* pRow = RowBox_t::pAllotRow();

  return pRow;
}

/*----------------------------------------------------------------------------*/
/* allot a row copying the rowSingle into it                                  */
/*----------------------------------------------------------------------------*/
Row* EachRowBuffer::pAllotRow(StreamDef* pStreamDef, i8 iVArgsBegin, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  // allot a row
  Row* pRow = EachRowBuffer::pAllotRow();

  Stz* szDst;

  if ( ! pRow )
    return nullptr;

  ASSERT(C_VARGS_BEGIN == iVArgsBegin);

  va_list xVarArgs;

  va_start(xVarArgs,iVArgsBegin);

  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(); i_Col++) {

    Column* pCol = pStreamDef->pGetColumn(i_Col);

    // STRING_DEPOSIT
    if (DM_VCH == pCol->eDatum()) {
      Stz* szVal = va_arg(xVarArgs,Stz*);
      if (szVal) {
        szDst = (Stz*)aAllotHead(StrLib::nSize(szVal->azChar()));

        // return early if no more space for the string
        if ( ! szDst )
          return nullptr;

        StrLib::vCopy((char*)szDst, szVal->azChar());

        pRow->vColVal_ofs(i_Col,szDst);
      }
      else {
        pRow->vSetColNull(i_Col);
      }
    }
    else {
      i8 iVal = va_arg(xVarArgs,i8);
      pRow->vColVal_int(i_Col,iVal);
    }
    // END STRING_DEPOSIT

  }

  i8 iVArgsEnd = va_arg(xVarArgs,i8);
  ASSERT(C_VARGS_END == iVArgsEnd);
  UNUSED(iVArgsEnd);

  va_end(xVarArgs);

  return pRow;
}

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/
i8 EachRowBuffer::iDecile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  i8 iSpaceUsed = nUsedBytes();
  i8 iDecile = IntLib::iDecile( iSpaceUsed , C_SIZE_POOL_BUFFER);

  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* deposit list of column values from fromRow into toRow                      */
/*----------------------------------------------------------------------------*/
void EachRowBuffer::vDepositRow(Row* pToRow, Row* pFromRow)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  Stz *szVal;
  d8 dVal;
  i8 iVal;

  ColPix* axaColPix = RowBox_t::axaColPix();

  for (i8 i_Col = 0; i_Col < nColumns(); i_Col++) {

    ColPix *pColPix = &axaColPix[i_Col];

    switch (pColPix->m_uDatum) {

      case DM_FLT:
        dVal = pFromRow->dColVal_flt(i_Col);
        pToRow->vColVal_flt(i_Col, dVal);
        break;

      case DM_BCD:
      case DM_BOL:
      case DM_DTE:
      case DM_TME:
      case DM_TSP:
      case DM_INT:
        iVal = pFromRow->iColVal_int(i_Col);
        pToRow->vColVal_int(i_Col, iVal);
        break;

      case DM_VCH:
        szVal = pFromRow->szColVal_ofs(i_Col);
        if (szVal) {
          Stz *szDst = (Stz *) aAllotHead(StrLib::nSize(szVal->azChar()));

          // there should be space for the string
          RASSERT(szDst);

          StrLib::vCopy((char *) szDst, szVal->azChar());

          pToRow->vColVal_ofs(i_Col, szDst);
        } else {
          pToRow->vSetColNull(i_Col);
        }
        break;

      default:
        NEVER_REACH;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EachRowBuffer* EachRowBuffer::pNextEachRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return _pEachRowBuffer(this->pNextLinkedBuffer());
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EachRowBuffer::vNextEachRowBuffer(EachRowBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  LinkedBuffer::vNextLinkedBuffer(pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EachRowBuffer* EachRowBuffer::pPrevEachRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return _pEachRowBuffer(this->pPrevLinkedBuffer());
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EachRowBuffer::vPrevEachRowBuffer(EachRowBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  vPrevLinkedBuffer(pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* print rows                                                                 */
/*----------------------------------------------------------------------------*/
void EachRowBuffer::vPrintRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RowPix* pPix = pRowPix();

  //pPix->vPrint();

  HV_Record hvRecord(pPix);

  i8 nRows = this->nRows();

  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    hvRecord->vToCsvRecord(pRowAddress(i_Row));
    hvRecord->vPrintAsRow();
  }
}

/*----------------------------------------------------------------------------*/
/* print rows                                                                 */
/*----------------------------------------------------------------------------*/
void EachRowBuffer::vPrintRows(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RowPix* pPix = pStreamDef->pRowPix();

  //pPix->vPrint();

  HV_Record hvRecord(pPix);

  // NOTE: We need to print only nItem no of rows
  // as the limitOp may have truncated the rows via setting the nItems()

  // get the first row
  Row* pRow = pFirstRow();
  HV_RowSingle hvRowSingle(pStreamDef);

  for (i8 i_Row = 0; i_Row < nItems(); i_Row++) {
    hvRowSingle->vCopyRow(pRow);
    hvRecord->vToCsvRecord(hvRowSingle->pRowCurrent());
    hvRecord->vPrintAsRow();

    // get the next row
    pRow = pNextRow(pRow);
  }
}

/*----------------------------------------------------------------------------*/
/* restore pointers are recv()                                                */
/*----------------------------------------------------------------------------*/
void EachRowBuffer::vRestorePointers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RowBox_t<C_WSP_EACHROWBUFFER>::vResetRowPix();
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void EachRowBuffer::vShow(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  i8 nRows = nItems();

  vTrace("EachRowBuffer: %p (buffer %lld): rows: %lld\n"_Stz,
         this,m_iBufferNo,nRows);

  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         pPrevLinkedBuffer(),pNextLinkedBuffer());

  vTrace("  partition: %lld \n"_Stz, m_tLglPtn.iAsI8());

  Row* pRow = pFirstRow();
  HV_RowSingle hvRowSingle(pStreamDef);

  HV_Record hvRecord(pStreamDef->pRowPix());
  while (pRow) {
    hvRowSingle->vCopyRow(pRow);
    hvRecord->vToCsvRecord(hvRowSingle->pRowCurrent());
    pRow = pNextRow(pRow);
    hvRecord->vPrintAsRow();
  }

  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get string area                                                            */
/*----------------------------------------------------------------------------*/
char* EachRowBuffer::azStrArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  char* azArea = ((char*)ayLatterHead()) + 1;

  return azArea;
}

/*----------------------------------------------------------------------------*/
/* convert this row buffer into one or more csv buffers                       */
/*----------------------------------------------------------------------------*/
CsvBuffer* EachRowBuffer::pToCsvBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  HV_Record hvRecord(pRowPix());

  CsvBuffer* pFirstCsvBuffer = new CsvBuffer;
  CsvBuffer* pCsvBuffer = pFirstCsvBuffer;

  Stz* szRecord = nullptr;
  i8 nSize = 1;

  i8 nRows = this->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {

    Row* aRow = pRowAddress(i_Row);

    Stz* szRow = hvRecord->sz(aRow,true);

    nSize = szRow->nSize();

    if (pCsvBuffer->nFreeBytes() < nSize + 1) {
      CsvBuffer* pNewCsvBuffer = new CsvBuffer;
      pCsvBuffer->vNextLinkedBuffer(pNewCsvBuffer);
      pCsvBuffer = pNewCsvBuffer;
    }

    szRecord = pCsvBuffer->szAllotRecord(nSize - 1);

    StrLib::vCopyLen(szRecord->azChar(),szRow->azChar(),nSize - 1);
  }

  if (szRecord)
    ((char*)szRecord)[nSize-1] = '\0';

  return pFirstCsvBuffer;
}

/*----------------------------------------------------------------------------*/
/* truncate this buffer by removing all entries                               */
/*----------------------------------------------------------------------------*/
void EachRowBuffer::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* verify this row buffer                                                 TBD */
/*----------------------------------------------------------------------------*/
bool EachRowBuffer::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

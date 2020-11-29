/*============================================================================*/
/* RowSlab.hpp -- Row Slab                                                    */
/*============================================================================*/

#include "Rows.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Rows,RowSlab,CN_ROWSLAB)
__aNewFunction(RowSlab,CN_ROWSLAB)
__vDeleteFunction(RowSlab,CN_ROWSLAB)
__aFooterFunction(RowSlab)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowSlab::RowSlab(void)
: RowWsp(CN_ROWSLAB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  MemLib::vSet(m_caModel,' ',sizeof(m_caModel),CN_ROWSLAB);
  m_caModel[sizeof(m_caModel) - 1] = '\0';
  
  StrLib::vSprint(m_caModel,"RapidsSE %s SeFormat %s"_Stz,
                  RAPIDSSE_VERSION_STRING,
                  RAPIDSSE_FORMAT_STRING);
  
  m_iSeVersion = RAPIDSSE_VERSION_NUMBER;
  m_iSeFormat  = RAPIDSSE_FORMAT_NUMBER;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_i_RowCrs  = C_UNKNOWN;
  m_aRowCur   = nullptr;
  m_i_ColCrs  = C_UNKNOWN;
    
  // m_oStreamPix
  
  m_nRowStringBytes = 0;
  
  MemLib::vZero(m__PadToCacheLine,sizeof(m__PadToCacheLine),CN_ROWSLAB);
  
  // m_oNmlStr

#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ROWSLAB);
#endif

  _checkInitialised_(CN_ROWSLAB);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowSlab::~RowSlab(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* get number of columns                                                      */
/*----------------------------------------------------------------------------*/
i8 RowSlab::nCols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return m_oStreamPix.m_nCols;
}

/*----------------------------------------------------------------------------*/
/* get number of extra columns                                                */
/*----------------------------------------------------------------------------*/
i8 RowSlab::nExtraCols(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return m_oStreamPix.m_nExtraCols;
}

/*----------------------------------------------------------------------------*/
/* get the condensed column pix array                                         */
/*----------------------------------------------------------------------------*/
ColPix* RowSlab::axaColPix(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  return m_oStreamPix.m_xaColPix;
}

/*----------------------------------------------------------------------------*/
/* get i_cell of cell[0] for given row                                        */
/*----------------------------------------------------------------------------*/
i8 RowSlab::i_Cell0(i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  //i8 iCell0_row = (i_Row + 1) * (m_oStreamPix.m_nCols);
  //i8 i_RowCell0 = C_CELL_BOUNDARY - iCell0_row;
  
  i8 iDummy_row = (i_Row + 1) * (m_oStreamPix.m_nCols + 1 /* dummy */ );
  i8 i_Dummy = C_CELL_BOUNDARY - iDummy_row;
  
  i8 i_RowCell0 = i_Dummy++;
  
  return i_RowCell0;
}

/*----------------------------------------------------------------------------*/
/* copy given stream pix into the stream pix in this rsb                      */
/*----------------------------------------------------------------------------*/
void RowSlab::vCopyPix(StreamPix* pStreamPixSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  MemLib::vCopy(&m_oStreamPix,pStreamPixSrc,sizeof(StreamPix),CN_ROWSLAB);
}

/*----------------------------------------------------------------------------*/
/* copy given source rsb into this current row of this row slab               */
/*----------------------------------------------------------------------------*/
void RowSlab::vCopyRsb(Row* aRowSrc, bool bSrcHasPtnHash, bool bCopyPtnHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  i8 iVal;
  Stz* szSrcStz;
  
  i8 nCols = m_oStreamPix.m_nCols;
  
  i8 i_Col;
  
  for (i_Col = 0; i_Col < nCols; i_Col++) {
    
    eDM eDatum = (eDM)m_oStreamPix.m_xaColPix[i_Col].m_uDatum;
    
    if (DM_VCH == eDatum) {
       // vch datum

      if (C_DATUM_NULL == aRowSrc->iColVal_int(i_Col)) {
        vDeposit(C_DATUM_NULL);
      }
      else {
        szSrcStz = aRowSrc->szVal_rsb(i_Col);
        vDeposit(szSrcStz);
      }
    }
    else {
      // all others
     
      iVal = aRowSrc->iColVal_int(i_Col);
      vDeposit(iVal);
    }
  }

  if (bSrcHasPtnHash && bCopyPtnHash) {
    iVal = aRowSrc->iColVal_int(i_Col);
    vDeposit(iVal);
  }
  
//  Row* aRowDst = aRowAdr(nRows() - 1);
//  SP_RowPix spPix(nCols,m_oStreamPix.m_xaColPix);
//  vTrace("CopyRsb: %s\n"_Stz,spPix.sz(aRowDst));
}

/*----------------------------------------------------------------------------*/
/* return column value from rsb for given column number                       */
/*----------------------------------------------------------------------------*/
i8 RowSlab::iColumn(Row* aRow, i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  i8 iVal;
  Stz* szSrcStz;
    
  eDM eDatum = (eDM)m_oStreamPix.m_xaColPix[i_Col].m_uDatum;
    
  if (DM_VCH == eDatum) {
    // vch datum
    
    if (C_DATUM_NULL == aRow->iColVal_int(i_Col)) {
      return C_DATUM_NULL;
    }
    else {
      szSrcStz = aRow->szVal_rsb(i_Col);
      return (i8)szSrcStz;
    }
  }
  else {
    // all others
    
    iVal = aRow->iColVal_int(i_Col);
    return iVal;
  }
}

/*----------------------------------------------------------------------------*/
/* discard current invalid/incomplete row                                     */
/*----------------------------------------------------------------------------*/
void RowSlab::vDiscardInvalidRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  // undo the row number increments of vNewRow()
  m_nItems--;
  m_i_RowCrs--;
  
  // return tail bytes allotted for cells of current row in vNewRow()
  i8 nCellBytes = nRowProperBytes();
  m_xWsp.vGiveTail(nCellBytes);
  
  // return head bytes allotted for strings deposited in vDepositString_end()
  if (m_nRowStringBytes) {
    m_xWsp.vGiveHead(m_nRowStringBytes);
    m_nRowStringBytes = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* dump given row                                                             */
/*----------------------------------------------------------------------------*/
void RowSlab::vDumpAllRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  i8 nRows = this->nRows();
  vTrace("RowSlab of %lld rows...\n"_Stz,nRows);
  for (i8 i_Row = 0; i_Row < nRows; i_Row++)
    vDumpRow(i_Row);
}

/*----------------------------------------------------------------------------*/
/* dump normalised string table                                               */
/*----------------------------------------------------------------------------*/
void RowSlab::vDumpNst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  m_oNmlStr.vDump();
}

/*----------------------------------------------------------------------------*/
/* dump given row                                                             */
/*----------------------------------------------------------------------------*/
void RowSlab::vDumpRow(i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  Row* aRow = this->aRowAdr(i_Row);
  vDumpRow(i_Row,aRow);
}

/*----------------------------------------------------------------------------*/
/* dump given row                                                             */
/*----------------------------------------------------------------------------*/
void RowSlab::vDumpRow(i8 i_Row, Row* aRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  SP_RowPix spRowPix(this->nCols(),this->axaColPix());
  
  Stz* szRow = spRowPix.sz(aRow);
  vTrace("[%lld] %s\n"_Stz,i_Row,szRow);
}

/*----------------------------------------------------------------------------*/
/* get number of rows                                                         */
/*----------------------------------------------------------------------------*/
i8 RowSlab::nFreeSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  i8 nFree = m_xWsp.nFree();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* initilize col pix                                                          */
/*----------------------------------------------------------------------------*/
void RowSlab::vInitPix(StreamDef* pStreamDef, bool bPtnHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
        
  pStreamDef->vToStreamPix(&m_oStreamPix,bPtnHash);
}

/*----------------------------------------------------------------------------*/
/* start of new row                                                           */
/*----------------------------------------------------------------------------*/
bool RowSlab::bNewRow(Row** paRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  i8 nBytesAvail = m_xWsp.nFree() - C_NML_STR_SPACE - (i8)C_BYTES_PER_CACHE_LINE;
  
  if (nBytesAvail < m_oStreamPix.m_nMaxRowSize + /* m_iDummy */ (i8)sizeof(i8))
    return false;
  
  m_nItems++;

  m_i_RowCrs++;
  
  m_aRowCur = aRowAdr(m_i_RowCrs);
  
  *paRow = m_aRowCur;
  
  m_i_ColCrs  = 0;
  
  i8 nCellBytes = nRowProperBytes();
  m_xWsp.vTakeTail(nCellBytes);
  
  m_nRowStringBytes = 0;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* start of new row                                                           */
/*----------------------------------------------------------------------------*/
bool RowSlab::bNewRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
    
  Row* aRow;
  
  bool b = bNewRow(&aRow);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get normalised string set                                                  */
/*----------------------------------------------------------------------------*/
NmlStr* RowSlab::pNmlStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  return & m_oNmlStr;
}

/*----------------------------------------------------------------------------*/
/* normalize repeating strings                                                */
/*----------------------------------------------------------------------------*/
void RowSlab::vNormaliseStrings(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  //m_oNmlStr.vDump();

  i8 nTimes = 0;
  i8 nRows  = this->nRows();
  i8 nCols  = this->nCols();
  
  if (nCols < 14)
    return;
  
  if (nRows < 250)
    return;
  
  //vTrace("NormaliseStrings %lld rows\n"_Stz,nRows);
  
//for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
  for (i8 i_Col = 13; i_Col < 14; i_Col++) {

    eDM eDatum = (eDM)m_oStreamPix.m_xaColPix[i_Col].m_uDatum;
    
    if (DM_VCH != eDatum)
      continue;
    

    for (i8 i_OuterRow = 0; i_OuterRow < 100; i_OuterRow++) {
      
      Row* aOuterRsb = aRowAdr(i_OuterRow);
      
      if (C_DATUM_NULL == aOuterRsb->iColVal_int(i_Col))
        continue;

      Stz* szOuterStz = aOuterRsb->szVal_rsb(i_Col);
      //vTrace("Outer: %s\n"_Stz,szOuterStz);
      
      i8 nLength = szOuterStz->nLength();
      if (nLength < 8)
        continue;
      
      for (i8 i_InnerRow = i_OuterRow + 1;
           i_InnerRow < i_OuterRow + 99;
           i_InnerRow++) {

        //vTrace("NormaliseStrings outer %lld inner %lld\n"_Stz,i_OuterRow,i_InnerRow);
        
        Row* aInnerRsb = aRowAdr(i_InnerRow);

        if (C_DATUM_NULL == aInnerRsb->iColVal_int(i_Col))
          continue;

        Stz* szInnerStz = aInnerRsb->szVal_rsb(i_Col);
        //vTrace("Inner: %s\n"_Stz,szInnerStz);

        if (! szInnerStz->bEqual(szOuterStz))
          continue;

        //vTrace("Testing : %lld - %lld : %s\n"_Stz,i_OuterRow,i_InnerRow,szOuterStz);
        if (! m_oNmlStr.szFind(szOuterStz)) {
          m_oNmlStr.szAdd(szOuterStz);
          if (++nTimes >= 3)
            goto L_Exit;
        }
        
        break;
        
      } // each inner row
      
    } // each outer row

  } // each col
  
L_Exit:
  ; //m_oNmlStr.vDump();
}

/*----------------------------------------------------------------------------*/
/* get the offset of rsb                                                      */
/*----------------------------------------------------------------------------*/
i8 RowSlab::iOfsOfRsb(Row* aRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  i8 iOfs = m_xWsp.iOffset((byte*)aRow);
  
  return iOfs;
}

/*----------------------------------------------------------------------------*/
/* position on given cell in current rsb                                      */
/*----------------------------------------------------------------------------*/
void RowSlab::vPositionOnCell(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  m_i_ColCrs = i_Col;
}

/*----------------------------------------------------------------------------*/
/* is there a ptn hash?                                                       */
/*----------------------------------------------------------------------------*/
bool RowSlab::bPtnHash(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  return m_oStreamPix.m_bPtnHash;
}

/*----------------------------------------------------------------------------*/
/* get the partition of rsb                                                   */
/*----------------------------------------------------------------------------*/
i8 RowSlab::iPtnOfRsb(Row* aRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  i8 i_HshCol = m_oStreamPix.m_nCols - 1;
    
  i8 i_Ptn = aRow->iColVal_int(i_HshCol);
  
  return i_Ptn;
}

/*----------------------------------------------------------------------------*/
/* reset this rowslab                                                         */
/*----------------------------------------------------------------------------*/
void RowSlab::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  m_nItems    = 0;
  m_i_RowCrs  = C_UNKNOWN;
  m_aRowCur   = nullptr;
  m_i_ColCrs  = C_UNKNOWN;

  m_xWsp.vReset();
}

/*----------------------------------------------------------------------------*/
/* get bytes needed to hold row cells proper (i.e. without strings)           */
/*----------------------------------------------------------------------------*/
i8 RowSlab::nRowProperBytes(void)
{
  i8 nBytes = (m_oStreamPix.m_nCols + 1 /* dummy */ ) * C_BYTES_PER_WORD;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get address of an rsb                                                      */
/*----------------------------------------------------------------------------*/
Row* RowSlab::aRowAdr(i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
    
  /*           v row[1]       v row[0]
       -------*----+----+----*----+----+----*
              | c0 : c1 : cN | c0 : c1 : cN |///boundary
       -------*----+----+----*----+----+----*
   */
  
  i8 i_RowCell0 = i_Cell0(i_Row);
    
  m8* amCellVec = amSlabCellVec();
  Row* aRow = (Row*) & amCellVec[i_RowCell0];
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* get address of an rsb by its offset                                        */
/*----------------------------------------------------------------------------*/
Row* RowSlab::aRowByOfs(i8 iOfs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  Row* aRow = aAdd<Row*>(m_xWsp.ayArea(),iOfs);
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* get address of an rsb by its offset                                        */
/*----------------------------------------------------------------------------*/
void RowSlab::vSetRow(i8 iRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  m_i_RowCrs = iRow;
}

/*----------------------------------------------------------------------------*/
/* get number of rows                                                         */
/*----------------------------------------------------------------------------*/
i8 RowSlab::nRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* dump slap and report                                                       */
/*----------------------------------------------------------------------------*/
void RowSlab::vSlabReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
 
  s32 sSlab_magn;  MagLib::vByteMag(C_ROWSLAB_WSP, sSlab_magn);
  s32 sUsed_magn;  MagLib::vByteMag(m_xWsp.nUsed(),sUsed_magn);
  s32 sFree_magn;  MagLib::vByteMag(m_xWsp.nFree(),sFree_magn);

  vTrace("RowSlab space %s, used %s, free %s %lld%%; "
         "rows %lld, %lld(B) per row\n"_Stz,
         
         sSlab_magn.szStz(),
         sUsed_magn.szStz(),
         sFree_magn.szStz(),
         (m_xWsp.nFree() * 100) / C_ROWSLAB_WSP,
         
         nRows(),
         m_xWsp.nUsed() / nRows()
         );
}

/*----------------------------------------------------------------------------*/
/* get the condensed stream def                                               */
/*----------------------------------------------------------------------------*/
StreamPix* RowSlab::aStreamPix(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  return & m_oStreamPix;
}

/*----------------------------------------------------------------------------*/
/* mem zero the given rsb                                                     */
/*----------------------------------------------------------------------------*/
void RowSlab::vZeroRsb(Row *aRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  i8 nCellBytes = m_oStreamPix.m_nCols * C_BYTES_PER_WORD;

  MemLib::vZero(aRow,nCellBytes,CN_ROWSLAB);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the cell vector for this slab                                          */
/*----------------------------------------------------------------------------*/
m8* RowSlab::amSlabCellVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

  m8* amCellArea = (m8*)m_xWsp.ayArea();
  
  return amCellArea;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

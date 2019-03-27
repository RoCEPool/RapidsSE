/*============================================================================*/
/* Record.cpp -- Record                                                       */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Record* Data::_pRecord(void* aAddr)
{
  Record* pRecord = reinterpret_cast<Record*>(aAddr);
  
  return pRecord;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Record::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_INDEXTREE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Record::operator delete(void* aAddr)
{
  Record* pRecord = reinterpret_cast<Record*>(aAddr);
  
  HeapMem::vFree(pRecord);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Record::Record(void)
: Object(CN_RECORD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
  
  _checkInitialised_(CN_RECORD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_RECORD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Record::~Record(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append decimal datum to string                                             */
/*----------------------------------------------------------------------------*/
void Record::vAppendBcd(Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  m_sRecStr.vCatSprint("%lld"_Stz,(i8)wDatum);
}

/*----------------------------------------------------------------------------*/
/* append boolean datum to string                                             */
/*----------------------------------------------------------------------------*/
void Record::vAppendBol(Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  m_sRecStr.vConcat(wDatum ? "true"_Stz : "false"_Stz);
}

/*----------------------------------------------------------------------------*/
/* append date datum to string                                                */
/*----------------------------------------------------------------------------*/
void Record::vAppendDte(Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  Dte wDte;
  wDte.vAsI8(wDatum);
  
  s32 sDte;
  wDte.vSprint(sDte.aStz());
  
  m_sRecStr.vConcat(sDte.aStz());
}

/*----------------------------------------------------------------------------*/
/* append float datum to string                                               */
/*----------------------------------------------------------------------------*/
void Record::vAppendFlt(Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  m_sRecStr.vCatSprint("%g"_Stz,(d8)wDatum);
}

/*----------------------------------------------------------------------------*/
/* append integer datum to string                                             */
/*----------------------------------------------------------------------------*/
void Record::vAppendInt(Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  m_sRecStr.vCatSprint("%lld"_Stz,(i8)wDatum);
}

/*----------------------------------------------------------------------------*/
/* append interval datum to string                                            */
/*----------------------------------------------------------------------------*/
void Record::vAppendIvl(Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  m_sRecStr.vCatSprint("%lld"_Stz,(i8)wDatum);
}

/*----------------------------------------------------------------------------*/
/* append time datum to string                                                */
/*----------------------------------------------------------------------------*/
void Record::vAppendTme(Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  Tme wTme;
  wTme.vAsI8((i8)wDatum);
  
  s32 sTme;
  wTme.vSprint(sTme.aStz());
  
  m_sRecStr.vConcat(sTme.aStz());
}

/*----------------------------------------------------------------------------*/
/* append timestamp datum to string                                           */
/*----------------------------------------------------------------------------*/
void Record::vAppendTsp(Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  Tsp wTsp;
  wTsp.vAsI8((i8)wDatum);
  
  s32 sTsp;
  wTsp.vSprint(sTsp.aStz());
  
  m_sRecStr.vConcat(sTsp.aStz());
}

/*----------------------------------------------------------------------------*/
/* append varchar datum to string                                             */
/*----------------------------------------------------------------------------*/
void Record::vAppendVch(Column* pCol, w8 wDatum, Stz* szDatum)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWDECODER,_FOOTER_);
  
  s256 sBuf;
  
  Vch::vSprintQuotes(sBuf,szDatum);
  m_sRecStr.vConcat(sBuf.aStz());
}


/*----------------------------------------------------------------------------*/
/* get record string                                                          */
/*----------------------------------------------------------------------------*/
Stz* Record::szRecStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  return m_sRecStr.aStz();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Record::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_RECORD);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

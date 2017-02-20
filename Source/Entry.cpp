/*============================================================================*/
/* Entry.cpp -- index entry                                                   */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get total bytes required for entry                                         */
/*----------------------------------------------------------------------------*/
i8 Entry::iBytesRequired(i8 iDataBytes)
{
  i8 iBytes = sizeof(Entry) + sizeof(Footer) + iDataBytes;
  
  return iBytes;
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Entry::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Entry class cannot be new'd\n");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Entry::operator new(size_t iSize, Entry* pEntry)
{
  RASSERT(iSize);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(i8 iDataBytes)
: Header(CN_ENTRY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY);

  //m_xRowId
  m_nDataBytes = iDataBytes;

  _setFooter_();
  
  _checkInitialised_(CN_ENTRY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Entry::Entry(EntWsp& xEntWsp)
: Header(CN_ENTRY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY);
  
  _setFooter_();
  
  vEntry(xEntWsp);
  
  _checkInitialised_(CN_ENTRY);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Entry::~Entry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* show content                                                               */
/*----------------------------------------------------------------------------*/
void Entry::vContentDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  vTrace("Entry %p: Row %llu.%llu (%lld databytes)\n",
         this,
         m_xRowId.uBuffer(),
         m_xRowId.uRow(),
         m_nDataBytes
         );
}

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
/*----------------------------------------------------------------------------*/
RowId& Entry::xRowId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return m_xRowId;
}

/*----------------------------------------------------------------------------*/
/* set row id                                                                 */
/*----------------------------------------------------------------------------*/
void Entry::vRowId(RowId& xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  m_xRowId = xRowId;
}

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void Entry::vEntry(EntWsp& xEntWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);

  m_xRowId = xEntWsp.xRowId();

  char* pDst = pDataNext();
  char* pSrc = xEntWsp.pDataArea();
  i8 nBytes  = xEntWsp.nDataBytes();
  
  Memory::vCopy(pDst,pSrc,nBytes);
  
  m_nDataBytes = nBytes;
}

/*----------------------------------------------------------------------------*/
/* get key                                                                    */
/*----------------------------------------------------------------------------*/
Key& Entry::xKey(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  Key* pKey = reinterpret_cast<Key*>(pDataArea());

  return reinterpret_cast<Key&>(pKey);
}

/*----------------------------------------------------------------------------*/
/* set key                                                                    */
/*----------------------------------------------------------------------------*/
void Entry::vKey(Key& xKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  Key* pKey = reinterpret_cast<Key*>(pDataArea());

  pKey->vKey(xKey);
}

/*----------------------------------------------------------------------------*/
/* set key                                                                    */
/*----------------------------------------------------------------------------*/
void Entry::vKey(KeyWsp& xKeyWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  Key* pKey = reinterpret_cast<Key*>(pDataArea());
  
  pKey->vKey(xKeyWsp);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get data area                                                              */
/*----------------------------------------------------------------------------*/
char* Entry::pDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  char* pDataAddr = pcAdd(this,sizeof(Entry));
  
  return pPoolCheck<char*>(pDataAddr);
}

/*----------------------------------------------------------------------------*/
/* get data bytes                                                             */
/*----------------------------------------------------------------------------*/
i8 Entry::iDataBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  return m_nDataBytes;
}

/*----------------------------------------------------------------------------*/
/* set data bytes                                                             */
/*----------------------------------------------------------------------------*/
void Entry::vDataBytes(i8 nDataBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);
  
  m_nDataBytes = nDataBytes;
}

/*----------------------------------------------------------------------------*/
/* get next avaliable point in data area                                      */
/*----------------------------------------------------------------------------*/
char* Entry::pDataNext(void)
{
  return &pDataArea()[m_nDataBytes];
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this entry                                            */
/*----------------------------------------------------------------------------*/
i8 Entry::iSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY,_FOOTER_);

  i8 nBytes = sizeof(Entry) + sizeof(Footer) + m_nDataBytes;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Entry::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRY);
  
  Footer* pFooter = static_cast<Footer*>(
                      pvAdd(this,sizeof(Entry) + m_nDataBytes));
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

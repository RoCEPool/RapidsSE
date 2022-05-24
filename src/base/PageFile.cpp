
/*============================================================================*/
/* PageFile.cpp -- Instance of a page file                                    */
/*============================================================================*/

#include "BASE.h"
extern AttachMap* gpAttachMap;
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PageFile* BASE::_pPageFile(void* aAddr)
{
  PageFile* pPageFile = reinterpret_cast<PageFile*>(aAddr);

  RASSERT_TYPE(pPageFile,CN_PAGEFILE);

  return pPageFile;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PageFile::operator new(size_t)
{
  PageFile* pPageFile = (PageFile*)Heaps::pAllocate(CN_PAGEFILE);

  return pPoolCheck<void*>(pPageFile,__func__);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void PageFile::operator delete(void* pPageFileAddress)
{
  PageFile* pPageFile = static_cast<PageFile*>(pPageFileAddress);

  Heaps::vFree(pPageFile);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PageFile::PageFile(Stz* sPath, u8 uIndex)
    : Object(CN_PAGEFILE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGEFILE,_FOOTER_);

  m_uPFIndex = uIndex;
  m_szPFilePath = sPath;
  m_pFile = new File(this);
  m_uPFSize = C_SIZE_PAGE_FILE;
  m_uFirstUsableByte = sizeof(C_MAGIC);
  m_uWilderness = 0;
  m_uLastUsableByte = m_uPFSize - sizeof(C_FOOTER) - 1;

  vInitialise();

  _checkInitialised_(CN_PAGEFILE);
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PAGEFILE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PageFile::~PageFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGEFILE,_FOOTER_);
#ifdef WITH_PAGE_FILE
  if (m_pFile->bIsOpen()) {
    m_pFile->vClose();
  }
#endif
  delete m_pFile;
}

/*----------------------------------------------------------------------------*/
/* check or initialize footer in page file                                    */
/*----------------------------------------------------------------------------*/
void PageFile::vInitFooter(void)
{
#ifdef WITH_PAGE_FILE
  u8 uFooter;
  i8 iSeekOffset = m_uPFSize - sizeof(C_FOOTER);
  m_pFile->vSeek(iSeekOffset);
  m_pFile->nReadBytes(&uFooter, sizeof(C_FOOTER));

  if (uFooter != C_FOOTER) {
    uFooter = C_FOOTER;
    m_pFile->vSeek(iSeekOffset);
    bool bResult = m_pFile->bWriteBytes((void*)&uFooter,sizeof(C_FOOTER));
    ASSERT(bResult);
  }
#endif
}

/*----------------------------------------------------------------------------*/
/* check or initialize magic in page file                                     */
/*----------------------------------------------------------------------------*/
void PageFile::vInitMagic(void)
{
#ifdef WITH_PAGE_FILE
  u8 uMagic = 0;
  m_pFile->vSeekStart();
  m_pFile->nReadBytes(&uMagic, sizeof(C_MAGIC));

  if(uMagic != C_MAGIC) {
    uMagic = C_MAGIC;
    m_pFile->vSeekStart();
    bool bResult = m_pFile->bWriteBytes(&uMagic,sizeof(C_MAGIC));
    ASSERT(bResult);
  }
#endif
}

/*----------------------------------------------------------------------------*/
/* dump PageFile                                                              */
/*----------------------------------------------------------------------------*/
void PageFile::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(pObject);
  UNUSED(aTab);

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* write pool buffers to disk                                                 */
/*----------------------------------------------------------------------------*/
void PageFile::vInitialise(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGEFILE,_FOOTER_);
#ifdef WITH_PAGE_FILE
  //---------------------------------------------------------------------------
  // Sanity check page file path

  //---------------------------------------------------------------------------
  // Open page file to read and write

  if (0 == m_pFile->bOpenMode(m_szPFilePath, C_OPEN_READ_WRITE_EXIST)) {
    vTrace("Error opening page file\n"_Stz);
  }

  //---------------------------------------------------------------------------
  // Check if magic header and footer are written to file
  // If not, initialize file

  vInitMagic();

  vInitFooter();

  m_pFile->vFlush();

//  m_uFirstUsableByte = sizeof(C_MAGIC);
//
//  m_uLastUsableByte = m_uPFSize - sizeof(C_FOOTER) - 1;
  //---------------------------------------------------------------------------
  // Check which areas are written to?
#endif
}

#ifdef WITH_BUFFER_ID
/*----------------------------------------------------------------------------*/
/* get the next available address of the free disk chunk in this page file    */
/*----------------------------------------------------------------------------*/
BufferId PageFile::oNextFreeBufferId(void)
{
  BufferId oBufferId;

  if(C_DATUM_NULL == uNextFreeSubset()) {
    vTrace("Page File %u is full\n"_Stz, m_uPFIndex);
    oBufferId.m_iAsI8 = C_DATUM_NULL;
    return oBufferId;
  }

  oBufferId.vSetBuffSubscript(m_uWilderness);

  // If multiple threads request for BufferIds at the same time,
  // we need to ensure it happens sequentially.
  m_uWilderness++;

  oBufferId.vSetPageFile(m_uPFIndex);
  oBufferId.vSetIdFlag(1);

  return oBufferId;
}

/*----------------------------------------------------------------------------*/
/* get the next free subset in this page file                                 */
/*----------------------------------------------------------------------------*/
u8 PageFile::uNextFreeSubset(void)
{
  //Check if the end of a new free chunk would exceed the PageFile size
#ifdef WITH_PAGE_FILE
  if(
      m_uFirstUsableByte +
      C_SIZE_POOL_BUFFER +
      (m_uWilderness *
      C_SIZE_POOL_BUFFER) >
      m_uLastUsableByte
    ) {
    vTrace("Page File %u is full\n"_Stz, m_uPFIndex);
    return C_DATUM_NULL; // TODO Arjun: this is i8, what u8 value can we send?
  }
#endif
  return m_uWilderness;
}
/*----------------------------------------------------------------------------*/
/* write data buffer to disk                                                  */
/*----------------------------------------------------------------------------*/
void PageFile::vWriteDataBufferToPFile(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
#ifdef WITH_PAGE_FILE
  Timer oTimer;

  BufferId oBufferId = pRowBuffer->oGetBufferId();

  // Temporary till BufferId subset has correct values
//  i8 iSeekOffset = m_uFirstUsableByte + (AM::pFileMap()->m_uPageFileSubset * C_SIZE_POOL_BUFFER);
   i8 iSeekOffset = m_uFirstUsableByte + (oBufferId.m_uBuffSubscript * C_SIZE_POOL_BUFFER);

  if(iSeekOffset + C_SIZE_POOL_BUFFER > m_uLastUsableByte)
  {
    vTrace("Error: writing buffer at %lld may exceed page file size %llu\n"_Stz, iSeekOffset, m_uPFSize);
    return;
  }
//lock
  m_pFile->vSeek(iSeekOffset);

  oTimer.vStart();

  bool bResult = m_pFile->bWriteBytes((void*)pRowBuffer,C_SIZE_POOL_BUFFER);
  ASSERT(bResult);
  oTimer.vUpdate();
//unlock
  vTrace("Writing data buffer %p took %lld ns\n"_Stz, pRowBuffer, oTimer.iNanoseconds());
#endif
}

/*----------------------------------------------------------------------------*/
/* read data buffer from disk                                                 */
/*----------------------------------------------------------------------------*/
RowBuffer* PageFile::pReadDataBufferFromPFile(BufferId oBufferId)
{
  _precondition_(_ADDRESS_,_MAGIC_);
#ifdef WITH_PAGE_FILE
  Timer oTimer;

  // Temporary till BufferId subset has correct values
//  i8 iSeekOffset = m_uFirstUsableByte + (0 * C_SIZE_POOL_BUFFER);

  i8 iSeekOffset = m_uFirstUsableByte + (oBufferId.m_uBuffSubscript * C_SIZE_POOL_BUFFER);//TODO Arjun: Check i8 and u8 overflows

  if(iSeekOffset + C_SIZE_POOL_BUFFER > m_uLastUsableByte)
  {
    vTrace("Error: reading buffer at %lld, may read beyond page file size %llu\n"_Stz, iSeekOffset, m_uPFSize);
    return nullptr;
  }

  RowBuffer* pRowBuffer = new RowBuffer;

//lock
  m_pFile->vSeek(iSeekOffset);

  oTimer.vStart();

  bool bResult = m_pFile->nReadBytes((void*)pRowBuffer,C_SIZE_POOL_BUFFER);
// BufferNo and other members of RowBuffer are overwritten
// Need to update those values

  ASSERT(bResult);
  oTimer.vUpdate();
//unlock
  vTrace("Reading data buffer %p took %lld ns\n"_Stz, pRowBuffer, oTimer.iNanoseconds());

  return pRowBuffer;
#else
  return nullptr;
#endif
}
#endif
/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PageFile::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGEFILE);

  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
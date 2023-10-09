
/*============================================================================*/
/* PageFile.cpp -- Instance of a page file                                    */
/*============================================================================*/

#include "BASE.h"
#include "CmdLineArgs.hpp"

extern AttachMap* gpAttachMap;
extern CmdLineArgs  gxCmdLineArgs;

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
//  m_uPFSize = C_SIZE_PAGE_FILE;
  m_uPFSize = (u8) gxCmdLineArgs.m_xSysArgs.m_iRegionSize * 10;
  m_uFirstUsableByte = 0;
  m_uWilderness = 0;
  m_uLastUsableByte = m_uPFSize - sizeof(C_FOOTER) - 1;
  MemLib::vZero(m_caBufferHeader, sizeof(m_caBufferHeader), CN_NONE);
  m_aHeader = (Header*) & m_caBufferHeader[0];
  m_pObject = reinterpret_cast<Object*>(m_aHeader);

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

  if (m_pFile->bIsOpen()) {
    m_pFile->vClose();
  }

  delete m_pFile;
}

/*----------------------------------------------------------------------------*/
/* check or initialize footer in page file                                    */
/*----------------------------------------------------------------------------*/
void PageFile::vInitFooter(void)
{

  u8 uFooter;
  i8 iSeekOffset = m_uPFSize - sizeof(C_FOOTER);
  m_pFile->vSeek(iSeekOffset);
  m_pFile->nReadBytes(&uFooter, sizeof(C_FOOTER));

  if (uFooter != C_FOOTER) {
    uFooter = C_FOOTER;
    m_pFile->vSeek(iSeekOffset);
    bool bResult = m_pFile->bWriteBytes((void*)&uFooter, sizeof(C_FOOTER));
    ASSERT(bResult);
    UNUSED(bResult);
  }

}

/*----------------------------------------------------------------------------*/
/* check or initialize magic in page file                                     */
/*----------------------------------------------------------------------------*/
void PageFile::vInitMagic(void)
{
  u8 uMagic = 0;
  if(m_pFile->iSize() == 0) {
    uMagic = C_MAGIC;
    bool bResult = m_pFile->bWriteBytes(&uMagic, sizeof(C_MAGIC));
    ASSERT(bResult);
    UNUSED(bResult);
    return;
  }

  m_pFile->vSeekStart();
  m_pFile->nReadBytes(&uMagic, sizeof(C_MAGIC));

  if(uMagic != C_MAGIC) {
    uMagic = C_MAGIC;
    m_pFile->vSeekStart();
    bool bResult = m_pFile->bWriteBytes(&uMagic, sizeof(C_MAGIC));
    ASSERT(bResult);
    UNUSED(bResult);
  }

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

//  vInitMagic();

//  vInitFooter();

//  m_uFirstUsableByte = sizeof(C_MAGIC);
//
//  m_uLastUsableByte = m_uPFSize - sizeof(C_FOOTER) - 1;
  //---------------------------------------------------------------------------
  // Check which areas are written to?

}

#ifdef WITH_PAGING
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
/* get the next free subset in this page file and increment the index         */
/*----------------------------------------------------------------------------*/
u8 PageFile::uNextFreeSubset(void)
{
  //Check if the end of a new free chunk would exceed the PageFile size

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

  return m_uWilderness++;
}
/*----------------------------------------------------------------------------*/
/* write data buffer to disk                                                  */
/*----------------------------------------------------------------------------*/
void PageFile::vWriteDataBufferToPFile(DataBuffer* pDataBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  //Calculate offset withing this pagefile to start reading from
  BufferId oBufferId = pDataBuffer->oGetBufferId();

  i8 iSeekOffset = m_uFirstUsableByte + (oBufferId.m_uSeekOffset * C_SIZE_POOL_BUFFER);

  if(iSeekOffset + C_SIZE_POOL_BUFFER > m_uLastUsableByte)
  {
    vTrace("Error: writing buffer at %lld may exceed page file size %llu\n"_Stz, iSeekOffset, m_uPFSize);
    return;
  }

  m_pFile->vSeek(iSeekOffset);

  bool bResult = m_pFile->bWriteBytes((void*)pDataBuffer, sizeof(CrwBuffer));
  RASSERT(bResult);

#ifdef WITH_PAGING_DEBUG
  vTrace("Successfully wrote data buffer %p with BufferId %p type %s\n"_Stz, pDataBuffer, oBufferId.m_pvAsAddress, aCN[pDataBuffer->eGetClassNo()]);
#endif
}

/*----------------------------------------------------------------------------*/
/* read data buffer from disk                                                 */
/*----------------------------------------------------------------------------*/
DataBuffer* PageFile::pReadDataBufferFromPFile(BufferId oBufferId)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  i8 iSeekOffset = m_uFirstUsableByte + (oBufferId.m_uSeekOffset * C_SIZE_POOL_BUFFER);//TODO Arjun: Check i8 and u8 overflows

  if(iSeekOffset + C_SIZE_POOL_BUFFER > m_uLastUsableByte)
  {
    vTrace("Error: reading buffer at %lld, may read beyond page file size %llu\n"_Stz, iSeekOffset, m_uPFSize);
    return nullptr;
  }

  i8 nSizeHeader = sizeof(Object);

  m_pFile->vSeek(iSeekOffset);

  // Read header information to get the persistent bit
  bool bResult = m_pFile->nReadBytes((void*)m_aHeader, nSizeHeader);
  RASSERT(bResult);
  RASSERT(m_pObject->eGetClassNo() == CN_ROWBUFFER ||
          m_pObject->eGetClassNo() == CN_CRWBUFFER);

  // Allocate a Buffer from the Transient or Persistent Large Heap
  // Depending on the persistent bit
  void* pPageInBuffer = nullptr;

  if(m_pObject->bGetPersistent()) {
    pPageInBuffer = Heaps::pAllocate(CN_PAGEINBUFFER, true);
  } else {
    pPageInBuffer = Heaps::pAllocate(CN_PAGEINBUFFER, false);
  }
  // Note: Allocation metrics will get dirty due to this
  // When freeing, we will be freeing a Crw or Row buffer instead of CN_PAGEINBUFFER

  // Copy the header into the allocated buffer
  MemLib::vCopy(pPageInBuffer, m_pObject, nSizeHeader, m_pObject->eGetClassNo());

  // Calculate body address
  void* pBody = aAdd<void *>(pPageInBuffer, nSizeHeader);

  // Read body
  i8 nSizeBody = ((i8) sizeof(CrwBuffer)) - nSizeHeader;
  bResult = m_pFile->nReadBytes((void*)pBody, nSizeBody);
  ASSERT(bResult);

  DataBuffer* pDataBuffer = reinterpret_cast<DataBuffer*>(pPageInBuffer);

  RASSERT(pDataBuffer->eGetClassNo() == CN_ROWBUFFER ||
          pDataBuffer->eGetClassNo() == CN_CRWBUFFER);

  // Re-calculate the BufferNo and set it after it has been overwritten
  i8 iPoolBufferNo = pDataBuffer->iBufferNo();
  pDataBuffer->vSetBufferNo(iPoolBufferNo);

  // Reset the bufferId in case it was set to zero during a pageout
  // TODO: Arjun - Hack remove
  if(pDataBuffer->oGetBufferId().m_iAsI8 != 0) {
    RASSERT(pDataBuffer->oGetBufferId().m_pvAsAddress == oBufferId.m_pvAsAddress);
  } else {
    pDataBuffer->vSetBufferId(oBufferId.m_pvAsAddress);
  }

  if(pDataBuffer->eGetClassNo() == CN_CRWBUFFER) {
    CrwBuffer* pCrwBuffer = _pCrwBuffer(pDataBuffer);
    pCrwBuffer->vRestoreNub();
//    pCrwBuffer->vDump();
  }

#ifdef WITH_PAGING_DEBUG
  vTrace("Successfully read data buffer %p with BufferId %p type %s\n"_Stz, pDataBuffer, oBufferId.m_pvAsAddress, aCN[pDataBuffer->eGetClassNo()]);
#endif
  return pDataBuffer;

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
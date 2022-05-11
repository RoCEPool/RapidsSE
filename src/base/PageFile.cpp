
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
PageFile::PageFile(Stz* sPath)
    : Object(CN_PAGEFILE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PAGEFILE,_FOOTER_);

  m_szPFilePath = sPath;
  m_pFile = new File(this);
  m_uPFSize = C_SIZE_PAGE_FILE;
  m_uFirstUsableByte = sizeof(C_MAGIC);
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
  if (m_pFile->bIsOpen()) {
    m_pFile->vClose();
  }
  delete m_pFile;
}

/*----------------------------------------------------------------------------*/
/* check or initialize footer in page file                                    */
/*----------------------------------------------------------------------------*/
void PageFile::vCheckFooter(void)
{
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
}

/*----------------------------------------------------------------------------*/
/* check or initialize magic in page file                                     */
/*----------------------------------------------------------------------------*/
void PageFile::vCheckMagic(void)
{
  u8 uMagic = 0;
  m_pFile->vSeekStart();
  m_pFile->nReadBytes(&uMagic, sizeof(C_MAGIC));

  if(uMagic != C_MAGIC)
  {
    uMagic = C_MAGIC;
    m_pFile->vSeekStart();
    bool bResult = m_pFile->bWriteBytes(&uMagic,sizeof(C_MAGIC));
    ASSERT(bResult);
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

  vCheckMagic();

  vCheckFooter();

  m_pFile->vFlush();

  m_uFirstUsableByte = sizeof(C_MAGIC);

  m_uLastUsableByte = m_uPFSize - sizeof(C_FOOTER) - 1;
  //---------------------------------------------------------------------------
  // Check which areas are written to?

}

/*----------------------------------------------------------------------------*/
/* write data buffer to disk                                                 */
/*----------------------------------------------------------------------------*/
void PageFile::vWriteDataBufferToPFile(RowBuffer* pRowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
#ifdef WITH_BUFFER_ID
  Timer oTimer;

  BufferId oBufferId = pRowBuffer->oGetBufferId();
  i8 iSeekOffset = m_uFirstUsableByte + (oBufferId.m_uBuffSubscript * C_SIZE_POOL_BUFFER);

  if(iSeekOffset + C_SIZE_POOL_BUFFER > m_uLastUsableByte)
  {
    vTrace("Error: writing buffer at %lld may exceed page file size %llu\n"_Stz, iSeekOffset, m_uPFSize);
    return;
  }

  m_pFile->vSeek(iSeekOffset);

  oTimer.vStart();

  bool bResult = m_pFile->bWriteBytes((void*)pRowBuffer,C_SIZE_POOL_BUFFER);
  ASSERT(bResult);
  oTimer.vUpdate();

  vTrace("Writing data buffer %p took %lld ns\n"_Stz, pRowBuffer, oTimer.iNanoseconds());
#endif
}

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
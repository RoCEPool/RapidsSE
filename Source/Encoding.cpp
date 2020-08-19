/*============================================================================*/
/* Encoding.cpp -- Row Encoding (intermediate representation of compact row)  */
/*============================================================================*/

#include "CompactBuffers.h"

extern EnvFlags gxEnvFlags;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompactBuffers,Encoding,CN_ENCODING)
__aNewFunction(Encoding,CN_ENCODING)
__vDeleteFunction(Encoding,CN_ENCODING)
__aFooterFunction(Encoding)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Encoding::Encoding(StreamDef* pStreamDef)
: Object(CN_ENCODING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
    
  if (gxEnvFlags.m_iMetaClue)
    m_pEncoding_v2 = new Encoding_v2(pStreamDef);
  else
    m_pEncoding_v1 = new Encoding_v1(pStreamDef);

  _checkInitialised_(CN_ENCODING);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Encoding::~Encoding(void) noexcept(false)
{
  delete m_pEncoding_v1;
  delete m_pEncoding_v2;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* how many bytes need in compact form                                        */
/*----------------------------------------------------------------------------*/
i8 Encoding::nBytesNeeded(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  if (gxEnvFlags.m_iMetaClue)
    return m_pEncoding_v2->nBytesNeeded();
  else
    return m_pEncoding_v1->nBytesNeeded();
}

/*----------------------------------------------------------------------------*/
/* encode inflated row                                                        */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeIrw(Row* pIrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  if (gxEnvFlags.m_iMetaClue)
    m_pEncoding_v2->vEncodeIrw(pIrw);
  else
    m_pEncoding_v1->vEncodeIrw(pIrw);
}

/*----------------------------------------------------------------------------*/
/* encode inflated row                                                        */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeRsb(Rsb* aRsb)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  if (gxEnvFlags.m_iMetaClue)
    m_pEncoding_v2->vEncodeRsb(aRsb);
  else
    m_pEncoding_v1->vEncodeRsb(aRsb);
      
}

/*----------------------------------------------------------------------------*/
/* encode signle row                                                          */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeSrw(RowSingle* pSrw)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  if (gxEnvFlags.m_iMetaClue)
    m_pEncoding_v2->vEncodeSrw(pSrw);
  else
    m_pEncoding_v1->vEncodeSrw(pSrw);
}

/*----------------------------------------------------------------------------*/
/* morph the given column encoding (to MC_STR_TAIL_2T)                        */
/*----------------------------------------------------------------------------*/
void Encoding::vMorphEncoding(_ColEnc* aColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  if (gxEnvFlags.m_iMetaClue)
    m_pEncoding_v2->vMorphEncoding(nullptr);
  else
    m_pEncoding_v1->vMorphEncoding(aColEnc);
      
}

/*----------------------------------------------------------------------------*/
/* does this encoding overflow the head?                                      */
/*----------------------------------------------------------------------------*/
bool Encoding::bOverFlowed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  if (gxEnvFlags.m_iMetaClue)
    return m_pEncoding_v2->bOverFlowed();
  else
    return m_pEncoding_v1->bOverFlowed();
}

/*----------------------------------------------------------------------------*/
/* calc the size of the over line                                             */
/*----------------------------------------------------------------------------*/
i8 Encoding::nOverSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  if (gxEnvFlags.m_iMetaClue)
    return m_pEncoding_v2->nOverSize();
  else
    return m_pEncoding_v1->nOverSize();
}

/*----------------------------------------------------------------------------*/
/* split at headSize for overflow (if necessary)                              */
/*----------------------------------------------------------------------------*/
void Encoding::vSplitOver(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  if (gxEnvFlags.m_iMetaClue)
    m_pEncoding_v2->vSplitOver();
  else
    m_pEncoding_v1->vSplitOver();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

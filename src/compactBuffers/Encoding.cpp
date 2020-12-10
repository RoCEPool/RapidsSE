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
  
  m_pEncoding_v2 = new Encoding_v2(pStreamDef);

  _checkInitialised_(CN_ENCODING);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Encoding::~Encoding(void) noexcept(false)
{
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
  
  return m_pEncoding_v2->nBytesNeeded();
}

/*----------------------------------------------------------------------------*/
/* encode inflated row                                                        */
/*----------------------------------------------------------------------------*/
void Encoding::vEncodeRow(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  m_pEncoding_v2->vEncodeRow(pRow);
}

/*----------------------------------------------------------------------------*/
/* morph the given column encoding (to MC_STR_TAIL_2T)                        */
/*----------------------------------------------------------------------------*/
void Encoding::vMorphEncoding(_ColEnc* aColEnc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  m_pEncoding_v2->vMorphEncoding(nullptr);
}

/*----------------------------------------------------------------------------*/
/* does this encoding overflow the head?                                      */
/*----------------------------------------------------------------------------*/
bool Encoding::bOverFlowed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  return m_pEncoding_v2->bOverFlowed();
}

/*----------------------------------------------------------------------------*/
/* calc the size of the over line                                             */
/*----------------------------------------------------------------------------*/
i8 Encoding::nOverSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  return m_pEncoding_v2->nOverSize();
}

/*----------------------------------------------------------------------------*/
/* split at headSize for overflow (if necessary)                              */
/*----------------------------------------------------------------------------*/
void Encoding::vSplitOver(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENCODING,_FOOTER_);
  
  m_pEncoding_v2->vSplitOver();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

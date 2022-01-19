
#include "BASE.h"
#include "BucketInfo.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,BucketInfo,CN_BUCKETINFO)
__aNewFunction(BucketInfo,CN_BUCKETINFO)
__vDeleteFunction(BucketInfo,CN_BUCKETINFO)
__aFooterFunction(BucketInfo)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BucketInfo::BucketInfo(void)
: Operand(CN_BUCKETINFO) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BUCKETINFO, _FOOTER_);

  m_sTableName = ""_Stz;
  m_iBucketNo = -1;
  m_tPtnId = 0;
  m_pCrwBuffer = nullptr;


  _checkInitialised_(CN_BUCKETINFO);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUCKETINFO,_FOOTER_);
}



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BucketInfo::BucketInfo(s32 sTblNm, PtnId tPtnId, i8 iBucket)
: Operand(CN_BUCKETINFO) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BUCKETINFO, _FOOTER_);

  m_sTableName = sTblNm;
  m_iBucketNo = iBucket;

  m_tPtnId = tPtnId;
  m_pCrwBuffer = nullptr;

  _checkInitialised_(CN_BUCKETINFO);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUCKETINFO,_FOOTER_);
}




/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BucketInfo::~BucketInfo(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETINFO,_FOOTER_);


}



/*----------------------------------------------------------------------------*/
/* get table name                                                             */
/*----------------------------------------------------------------------------*/
s32 BucketInfo::sTableName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETINFO,_FOOTER_);
  return m_sTableName;
}



/*----------------------------------------------------------------------------*/
/* get bucket number                                                          */
/*----------------------------------------------------------------------------*/
i8  BucketInfo::iBucketNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETINFO,_FOOTER_);
  return m_iBucketNo;
}


/*----------------------------------------------------------------------------*/
/* get partition number                                                       */
/*----------------------------------------------------------------------------*/
PtnId BucketInfo::tPtnId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETINFO,_FOOTER_);
  return m_tPtnId;
}


/*----------------------------------------------------------------------------*/
/* set rowbuffer                                                              */
/*----------------------------------------------------------------------------*/
void BucketInfo::vCrwBuffer(CrwBuffer* pCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETINFO,_FOOTER_);
  m_pCrwBuffer = pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* get rowbuffer                                                              */
/*----------------------------------------------------------------------------*/
CrwBuffer* BucketInfo::pCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETINFO,_FOOTER_);
  return m_pCrwBuffer;
}
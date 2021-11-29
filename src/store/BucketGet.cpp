
#include "BASE.h"
#include "BucketGet.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,BucketGet,CN_BUCKETGET)
__aNewFunction(BucketGet,CN_BUCKETGET)
__vDeleteFunction(BucketGet,CN_BUCKETGET)
__aFooterFunction(BucketGet)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BucketGet::BucketGet(void)
    : Operand(CN_BUCKETGET) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BUCKETGET, _FOOTER_);

  m_sTableName = ""_Stz;


  _checkInitialised_(CN_BUCKETGET);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUCKETGET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
/*
BucketGet::BucketGet(s32 sTableNm)
    : Operand(CN_BUCKETGET) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BUCKETGET, _FOOTER_);

  m_sTableName = sTableNm;


  _checkInitialised_(CN_BUCKETGET);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUCKETGET,_FOOTER_);
}*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BucketGet::BucketGet(i8 iBucket)
    : Operand(CN_BUCKETGET) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BUCKETGET, _FOOTER_);

  m_iBucketNo = iBucket;


  _checkInitialised_(CN_BUCKETGET);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUCKETGET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BucketGet::BucketGet(TablePtn* pTablePtn)
    : Operand(CN_BUCKETGET) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BUCKETGET, _FOOTER_);

  m_pTablePtn = pTablePtn;


  _checkInitialised_(CN_BUCKETGET);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUCKETGET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
BucketGet::BucketGet(TablePtn* pTablePtn, Bucket* pBucket)
: Operand(CN_BUCKETGET) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BUCKETGET, _FOOTER_);

  m_pTablePtn = pTablePtn;

  m_pBucket = pBucket;


  _checkInitialised_(CN_BUCKETGET);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_BUCKETGET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
BucketGet::~BucketGet(void) noexcept(false)
{
_precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETGET,_FOOTER_);


}

TablePtn* BucketGet::pTablePtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETGET,_FOOTER_);
  return m_pTablePtn;
}
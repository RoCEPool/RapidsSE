
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
BucketGet::BucketGet(s32 sTblNm, i8 iBucket)
    : Operand(CN_BUCKETGET) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_BUCKETGET, _FOOTER_);

  m_sTableName = sTblNm;
  m_iBucketNo = iBucket;

  m_pTablePtn = nullptr;
  m_pBucket  = nullptr;

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

  m_sTableName = ""_Stz;
  m_iBucketNo = -1;
  m_pBucket  = nullptr;

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

  m_sTableName = ""_Stz;
  m_iBucketNo = -1;

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

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TablePtn* BucketGet::pTablePtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETGET,_FOOTER_);
  return m_pTablePtn;
}

/*----------------------------------------------------------------------------*/
/* set table name                                                             */
/*----------------------------------------------------------------------------*/
void BucketGet::vTableName(s32 sTblNm)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETGET,_FOOTER_);
  m_sTableName = sTblNm;
}

/*----------------------------------------------------------------------------*/
/* get table name                                                             */
/*----------------------------------------------------------------------------*/
s32 BucketGet::sTableName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETGET,_FOOTER_);
  return m_sTableName;
}

/*----------------------------------------------------------------------------*/
/* set bucket number                                                          */
/*----------------------------------------------------------------------------*/
void BucketGet::vBucketNo(i8 iBktNo)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETGET,_FOOTER_);
  m_iBucketNo = iBktNo;
}

/*----------------------------------------------------------------------------*/
/* get bucket number                                                          */
/*----------------------------------------------------------------------------*/
i8  BucketGet::iBucketNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_, CN_BUCKETGET,_FOOTER_);
  return m_iBucketNo;
}
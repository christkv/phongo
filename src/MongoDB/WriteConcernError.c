/*
  +---------------------------------------------------------------------------+
  | PHP Driver for MongoDB                                                    |
  +---------------------------------------------------------------------------+
  | Copyright 2013-2014 MongoDB, Inc.                                         |
  |                                                                           |
  | Licensed under the Apache License, Version 2.0 (the "License");           |
  | you may not use this file except in compliance with the License.          |
  | You may obtain a copy of the License at                                   |
  |                                                                           |
  | http://www.apache.org/licenses/LICENSE-2.0                                |
  |                                                                           |
  | Unless required by applicable law or agreed to in writing, software       |
  | distributed under the License is distributed on an "AS IS" BASIS,         |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  |
  | See the License for the specific language governing permissions and       |
  | limitations under the License.                                            |
  +---------------------------------------------------------------------------+
  | Copyright (c) 2014, MongoDB, Inc.                                         |
  +---------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

/* External libs */
#include <bson.h>
#include <mongoc.h>

/* PHP Core stuff */
#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#include <Zend/zend_interfaces.h>
#include <ext/spl/spl_iterators.h>
/* Our Compatability header */
#include "php_compat_53.h"

/* Our stuffz */
#include "php_phongo.h"
#include "php_bson.h"


PHONGO_API zend_class_entry *php_phongo_writeconcernerror_ce;

/* {{{ proto integer WriteConcernError::getCode()
   Returns the MongoDB error code */
PHP_METHOD(WriteConcernError, getCode)
{
	php_phongo_writeconcernerror_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeconcernerror_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	RETURN_LONG(intern->code);
}
/* }}} */
/* {{{ proto array WriteConcernError::getInfo()
   Returns additional metadata for the error */
PHP_METHOD(WriteConcernError, getInfo)
{
	php_phongo_writeconcernerror_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeconcernerror_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	if (intern->info && Z_TYPE_P(intern->info) == IS_ARRAY) {
		RETURN_ZVAL(intern->info, 1, 0);
	}

	array_init(return_value);
}
/* }}} */
/* {{{ proto string WriteConcernError::getMessage()
   Returns the actual error message from the server */
PHP_METHOD(WriteConcernError, getMessage)
{
	php_phongo_writeconcernerror_t *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_writeconcernerror_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */

/**
 * Value object for a write concern error.
 */
/* {{{ MongoDB\WriteConcernError */

ZEND_BEGIN_ARG_INFO_EX(ai_WriteConcernError_getCode, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteConcernError_getInfo, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_WriteConcernError_getMessage, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_writeconcernerror_me[] = {
	PHP_ME(WriteConcernError, getCode, ai_WriteConcernError_getCode, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteConcernError, getInfo, ai_WriteConcernError_getInfo, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(WriteConcernError, getMessage, ai_WriteConcernError_getMessage, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ php_phongo_writeconcernerror_t object handlers */
static void php_phongo_writeconcernerror_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_writeconcernerror_t *intern = (php_phongo_writeconcernerror_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	if (intern->message) {
		efree(intern->message);
	}

	if (intern->info) {
		zval_ptr_dtor(&intern->info);
	}
	efree(intern);
} /* }}} */

zend_object_value php_phongo_writeconcernerror_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_writeconcernerror_t *intern;

	intern = (php_phongo_writeconcernerror_t *)emalloc(sizeof(php_phongo_writeconcernerror_t));
	memset(intern, 0, sizeof(php_phongo_writeconcernerror_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_writeconcernerror_free_object, NULL TSRMLS_CC);
	retval.handlers = phongo_get_std_object_handlers();

	return retval;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(WriteConcernError)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB", "WriteConcernError", php_phongo_writeconcernerror_me);
	ce.create_object = php_phongo_writeconcernerror_create_object;
	php_phongo_writeconcernerror_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_writeconcernerror_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;


	return SUCCESS;
}
/* }}} */



/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

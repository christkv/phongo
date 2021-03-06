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


PHONGO_API zend_class_entry *php_phongo_server_ce;

inline int server_populate(php_phongo_server_t *server);
zend_object_handlers php_phongo_handler_server;


/* {{{ proto MongoDB\Server Server::__construct(string $host, integer $port[, array $options = array()[, array $driverOptions = array()]])
   Constructs a new Server */
PHP_METHOD(Server, __construct)
{
	php_phongo_server_t      *intern;
	zend_error_handling       error_handling;
	char                     *host;
	int                       host_len;
	long                      port;
	zval                     *options = NULL;
	zval                     *driverOptions = NULL;
	mongoc_uri_t             *uri;
	void                   ***ctx = NULL;
	TSRMLS_SET_CTX(ctx);


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl|a!a!", &host, &host_len, &port, &options, &driverOptions) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	uri = mongoc_uri_new_for_host_port(host, port);
	intern->client = mongoc_client_new_from_uri(uri);
	if (!intern->client) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to parse MongoDB URI" TSRMLS_CC);
		return;
	}

	intern->host = mongoc_uri_get_hosts(uri);
	mongoc_client_set_stream_initiator(intern->client, phongo_stream_initiator, ctx);
}
/* }}} */
/* {{{ proto MongoDB\CommandResult Server::executeCommand(string $db, MongoDB\Command $command)
   Executes a command on this server */
PHP_METHOD(Server, executeCommand)
{
	php_phongo_server_t      *intern;
	zend_error_handling       error_handling;
	char                     *db;
	int                       db_len;
	zval                     *command;
	php_phongo_command_t     *cmd;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO", &db, &db_len, &command, php_phongo_command_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	cmd = (php_phongo_command_t *)zend_object_store_get_object(command TSRMLS_CC);
	phongo_execute_command(intern->client, db, cmd->bson, NULL, return_value, return_value_used TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\QueryResult Server::executeQuery(string $namespace, MongoDB\Query $zquery)
   Executes a Query */
PHP_METHOD(Server, executeQuery)
{
	php_phongo_server_t      *intern;
	zend_error_handling       error_handling;
	char                     *namespace;
	int                       namespace_len;
	zval                     *zquery;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO", &namespace, &namespace_len, &zquery, php_phongo_query_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	phongo_execute_query(intern->client, namespace, phongo_query_from_zval(zquery TSRMLS_CC), NULL, return_value, return_value_used TSRMLS_CC);
}
/* }}} */
/* {{{ proto MongoDB\WriteResult Server::executeWriteBatch(string $namespace, MongoDB\WriteBatch $zbatch)
   Executes a write operation batch (e.g. insert, update, delete) */
PHP_METHOD(Server, executeWriteBatch)
{
	php_phongo_server_t      *intern;
	zend_error_handling       error_handling;
	char                     *namespace;
	int                       namespace_len;
	zval                     *zbatch;
	php_phongo_writebatch_t  *batch;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sO", &namespace, &namespace_len, &zbatch, php_phongo_writebatch_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	batch = (php_phongo_writebatch_t *)zend_object_store_get_object(zbatch TSRMLS_CC);
	phongo_execute_write(intern->client, namespace, batch->batch, intern->hint, return_value, return_value_used TSRMLS_CC);
}
/* }}} */
/* {{{ proto string Server::getHost()
   Returns the hostname used to connect to this Server */
PHP_METHOD(Server, getHost)
{
	php_phongo_server_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	// FIXME: BUGBUG: this is a workaround as its not implemented yet :)
	server_populate(intern);
	RETURN_STRING(intern->host->host, 1);
}
/* }}} */
/* {{{ proto array Server::getInfo()
   Returns the last isMaster() result document */
PHP_METHOD(Server, getInfo)
{
	php_phongo_server_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */
/* {{{ proto integer Server::getLatency()
   Returns the last messured latency */
PHP_METHOD(Server, getLatency)
{
	php_phongo_server_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */
/* {{{ proto integer Server::getPort()
   Returns the port used to create this Server */
PHP_METHOD(Server, getPort)
{
	php_phongo_server_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);


	RETURN_LONG(intern->host->port);
}
/* }}} */
/* {{{ proto integer Server::getState()
   Returns the current state of the node (maintenece/startup/...) */
PHP_METHOD(Server, getState)
{
	php_phongo_server_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */
/* {{{ proto integer Server::getType()
   Returns the node type of this Server */
PHP_METHOD(Server, getType)
{
	php_phongo_server_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */
/* {{{ proto bool Server::isDelayed()
   Checks if this is a special "delayed" member of a RepilcaSet */
PHP_METHOD(Server, isDelayed)
{
	php_phongo_server_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */
/* {{{ proto bool Server::isPassive()
   Checks if this is a special passive node member of a ReplicaSet */
PHP_METHOD(Server, isPassive)
{
	php_phongo_server_t      *intern;
	zend_error_handling       error_handling;


	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling TSRMLS_CC);
	intern = (php_phongo_server_t *)zend_object_store_get_object(getThis() TSRMLS_CC);

	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling TSRMLS_CC);
		return;
	}
	zend_restore_error_handling(&error_handling TSRMLS_CC);

}
/* }}} */

/**
 * Server abstracts a socket connection to a single MongoDB server. The server
 * itself may be a mongod (stand-alone or replica set node) or mongos process.
 *
 * Users will typically not construct this class directly. The common use case
 * will be connection to a cluster of servers via a URI with the Manager class.
 *
 * This class does not utilize read preferences, since there is only a single
 * single socket on which to send a command, query, or write.
 *
 * Operation methods do not take socket-level options (e.g. socketTimeoutMS).
 * Those options should be specified during construction.
 */
/* {{{ MongoDB\Server */

ZEND_BEGIN_ARG_INFO_EX(ai_Server___construct, 0, 0, 2)
	ZEND_ARG_INFO(0, host)
	ZEND_ARG_INFO(0, port)
	ZEND_ARG_ARRAY_INFO(0, options, 0)
	ZEND_ARG_ARRAY_INFO(0, driverOptions, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, db)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Command, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeQuery, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zquery, MongoDB\\Query, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeWriteBatch, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zbatch, MongoDB\\WriteBatch, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_getHost, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_getInfo, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_getLatency, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_getPort, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_getState, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_getType, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_isDelayed, 0, 0, 0)
ZEND_END_ARG_INFO();

ZEND_BEGIN_ARG_INFO_EX(ai_Server_isPassive, 0, 0, 0)
ZEND_END_ARG_INFO();


static zend_function_entry php_phongo_server_me[] = {
	PHP_ME(Server, __construct, ai_Server___construct, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, executeCommand, ai_Server_executeCommand, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, executeQuery, ai_Server_executeQuery, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, executeWriteBatch, ai_Server_executeWriteBatch, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getHost, ai_Server_getHost, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getInfo, ai_Server_getInfo, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getLatency, ai_Server_getLatency, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getPort, ai_Server_getPort, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getState, ai_Server_getState, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, getType, ai_Server_getType, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, isDelayed, ai_Server_isDelayed, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_ME(Server, isPassive, ai_Server_isPassive, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};

/* }}} */


/* {{{ Other functions */
inline int server_populate(php_phongo_server_t *server)
{
	mongoc_host_list_t *host = NULL;
	host = (mongoc_host_list_t *) emalloc(sizeof(mongoc_host_list_t));

	strcpy(host->host, "localhost");
	host->port = 27017;

	server->host = host;

	return true;
}

zend_object_handlers* php_phongo_handlers_server() /* {{{ */
{
	return &php_phongo_handler_server;
} /* }}} */
static int php_phongo_server_compare_objects(zval *o1, zval *o2 TSRMLS_DC) /* {{{ */
{
    php_phongo_server_t *intern1;
    php_phongo_server_t *intern2;

    intern1 = (php_phongo_server_t *)zend_object_store_get_object(o1 TSRMLS_CC);
    intern2 = (php_phongo_server_t *)zend_object_store_get_object(o2 TSRMLS_CC);

	/* FIXME: BUGBUG: We need a way to get mongoc_host from WriteResults */
	if (intern1 && intern2) {
		return 0;
	}
    /*
	if (!strcmp(intern1->host->host_and_port, intern2->host->host_and_port)) {
		return 0;
	}
     */

	return 1;
} /* }}} */
/* }}} */
/* {{{ php_phongo_server_t object handlers */
static void php_phongo_server_free_object(void *object TSRMLS_DC) /* {{{ */
{
	php_phongo_server_t *intern = (php_phongo_server_t*)object;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	efree(intern);
} /* }}} */

zend_object_value php_phongo_server_create_object(zend_class_entry *class_type TSRMLS_DC) /* {{{ */
{
	zend_object_value retval;
	php_phongo_server_t *intern;

	intern = (php_phongo_server_t *)emalloc(sizeof(php_phongo_server_t));
	memset(intern, 0, sizeof(php_phongo_server_t));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	object_properties_init(&intern->std, class_type);

	retval.handle = zend_objects_store_put(intern, (zend_objects_store_dtor_t) zend_objects_destroy_object, php_phongo_server_free_object, NULL TSRMLS_CC);
	retval.handlers = php_phongo_handlers_server();

	return retval;
} /* }}} */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(Server)
{
	(void)type; /* We don't care if we are loaded via dl() or extension= */
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB", "Server", php_phongo_server_me);
	ce.create_object = php_phongo_server_create_object;
	php_phongo_server_ce = zend_register_internal_class(&ce TSRMLS_CC);
	php_phongo_server_ce->ce_flags |= ZEND_ACC_FINAL_CLASS;


	memcpy(&php_phongo_handler_server, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_phongo_handler_server.compare_objects = php_phongo_server_compare_objects;
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_MONGOS"), 0x01 TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_STANDALONE"), 0x02 TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_ARBITER"), 0x03 TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_SECONDARY"), 0x04 TSRMLS_CC);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_PRIMARY"), 0x05 TSRMLS_CC);

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

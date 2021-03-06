<?php

namespace MongoDB;

use Countable;

class WriteBatch implements Countable
{
    /**
     * Constructs a new WriteBatch
     *
     * @param boolean $ordered Initialize (un-)ordered batch
     * @return WriteBatch
     */
    function __construct($ordered = true) {
        /*** CIMPL ***/
/*
	intern->batch = phongo_writebatch_init(ordered);
*/
        /*** CIMPL ***/
    }

    /**
     * Adds an insert operation to the batch
     *
     * The document's "_id" will be returned (either from the provided document
     * or a generated value from the driver).
     *
     * @param array|object $document Operation/document to add to insert
     * @return mixed Document identifier
     */
    function insert($document) {
        /*** CEF ***/
/*
	bson_t                   *bson;
	bson_t                   *bson_out = NULL;
	int                       bson_flags = PHONGO_BSON_ADD_ID;
*/
        /*** CEF ***/
        /*** CIMPL ***/
/*
	if (return_value_used) {
		bson_flags |= PHONGO_BSON_RETURN_ID;
	}

	bson = bson_new();
	zval_to_bson(document, bson_flags, bson, &bson_out TSRMLS_CC);
	mongoc_bulk_operation_insert(intern->batch, bson);
	bson_destroy(bson);

	if (bson_out && return_value_used) {
		bson_iter_t iter;

		if (bson_iter_init_find(&iter, bson_out, "_id")) {
			php_phongo_objectid_new_from_oid(return_value, bson_iter_oid(&iter) TSRMLS_CC);
			return;
		}
	}
*/
        /*** CIMPL ***/
    }

    /**
     * Adds an update operation to batch
     *
     * @param array|object $query         Update criteria
     * @param array|object $newObj        Update modifier or replacement document
     * @param array        $updateOptions Update options (e.g. "upsert")
     */
    function update($query, $newObj, array $updateOptions = null)
    {
        /*** CEF ***/
/*
	mongoc_update_flags_t     limit = 0;
	zend_bool                 upsert = 0;
	bson_t                   *bquery;
	bson_t                   *bupdate;
*/
        /*** CEF ***/
        $updateOptions = array_merge(
            array('multi' => false, 'upsert' => false),
            $updateOptions
        );
        /*** CIMPL ***/
/*
	bquery = bson_new();
	bupdate = bson_new();

	zval_to_bson(query, PHONGO_BSON_NONE, bquery, NULL TSRMLS_CC);
	zval_to_bson(newObj, PHONGO_BSON_NONE, bupdate, NULL TSRMLS_CC);

	if (updateOptions) {
		limit = php_array_fetch_bool(updateOptions, "limit");
		upsert = php_array_fetch_bool(updateOptions, "upsert");
	}

	if (limit) {
		mongoc_bulk_operation_update_one(intern->batch, bquery, bupdate, upsert);
	} else {
		mongoc_bulk_operation_update(intern->batch, bquery, bupdate, upsert);
	}

	bson_destroy(bquery);
	bson_destroy(bupdate);
*/
        /*** CIMPL ***/
    }

    /**
     * Adds a delete operation to the batch
     *
     * @param array|object $query         Deletion criteria
     * @param array        $deleteOptions Deletion options (e.g. "limit")
     */
    function delete($query, array $deleteOptions = null)
    {
        /*** CEF ***/
/*
	bson_t                   *bson;
*/
        /*** CEF ***/
        /* TODO: Decide if we should default to 0 (i.e. "all"), as in 1.x, or if
         * an explicit "limit" option must be required. Keeping $deleteOptions
         * as an array, rather than a single integer option for "limit", seems
         * wise; if the "delete" command ever takes additional options, we would
         * not need to change our API.
         */
        $deleteOptions = array_merge(
            array('limit' => 0),
            $deleteOptions
        );
        /*** CIMPL ***/
/*
	bson = bson_new();
	zval_to_bson(query, PHONGO_BSON_NONE, bson, NULL TSRMLS_CC);

	if (deleteOptions && php_array_fetch_bool(deleteOptions, "limit")) {
		mongoc_bulk_operation_remove_one(intern->batch, bson);
	} else {
		mongoc_bulk_operation_remove(intern->batch, bson);
	}

	bson_destroy(bson);
*/
        /*** CIMPL ***/
    }

    /**
     * Returns the number of operations that have been added to the batch
     *
     * @return integer
     */
    function count() {}
}

$WriteBatch["headers"][] = '"php_array.h"';

$WriteBatch["free"] = <<< EOF
	if (intern->batch) {
		mongoc_bulk_operation_destroy(intern->batch);
	}

EOF;

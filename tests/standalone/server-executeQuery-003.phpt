--TEST--
MongoDB\Server::executeQuery() with modifiers and empty filter
--SKIPIF--
<?php require "tests/utils/basic-skipif.inc" ?>
--FILE--
<?php
require_once "tests/utils/basic.inc";

$server = new MongoDB\Server('localhost', 27017);

// load fixtures for test
$batch = new \MongoDB\WriteBatch();
$batch->insert(array('_id' => 1, 'x' => 2, 'y' => 3));
$batch->insert(array('_id' => 2, 'x' => 3, 'y' => 4));
$batch->insert(array('_id' => 3, 'x' => 4, 'y' => 5));
$server->executeWriteBatch(NS, $batch);

$query = new MongoDB\Query(array(), array('modifiers' => array('$comment' => 'foo')));
$cursor = $server->executeQuery(NS, $query);

var_dump($cursor instanceof MongoDB\QueryResult);
var_dump($server == $cursor->getServer());
var_dump(iterator_to_array($cursor));

?>
===DONE===
<?php exit(0); ?>
--EXPECT--
bool(true)
bool(true)
array(3) {
  [0]=>
  array(3) {
    ["_id"]=>
    int(1)
    ["x"]=>
    int(2)
    ["y"]=>
    int(3)
  }
  [1]=>
  array(3) {
    ["_id"]=>
    int(2)
    ["x"]=>
    int(3)
    ["y"]=>
    int(4)
  }
  [2]=>
  array(3) {
    ["_id"]=>
    int(3)
    ["x"]=>
    int(4)
    ["y"]=>
    int(5)
  }
}
===DONE===

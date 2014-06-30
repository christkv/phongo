<?php

namespace MongoDB;

/**
 * Result implementation that is returned after executing a Query.
 *
 * Wraps the result, allowing the user to overwrite which class should be used
 * to iterate over the result object
 */
final class Result implements \IteratorAggregate
{
    private $iterator = "QueryCursor || CommandCursor";


    /**
     * Construct a new MongoDB\Result object
     *
     * @param Server   $server
     * @param CursorId $cursorId
     * @param array    $firstBatch
     */
    public function __construct(Server $server, CursorId $cursorId, array $firstBatch)
    {
        $this->server = $server;
        $this->cursorId = $cursorId;
        $this->firstBatch = $firstBatch;
    }

    /**
     * Sets the classname of the iterator to use
     *
     * @param Cursor $classname
     * @return Result
     */
    public function setIteratorClass(Cursor $classname)
    {
        $this->iterator = $classname;
        return $this;
    }

    /**
     * Returns the Iteratable MongoDB\Cursor object
     *
     * @return Cursor
     */
    public function getIterator()
    {
        return new $this->iterator($stuff);
    }
}

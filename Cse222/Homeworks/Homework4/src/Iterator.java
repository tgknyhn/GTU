public interface Iterator<E> {
    /**
     * Returns true if it is not last element
     * @return True when next element exist
     */
    boolean hasNext();

    /**
     * Returns next element and iterates.
     * @return Next element
     */
    E next();

    /**
     * Removes element that current index points
     */
    void remove();

    /**
     * Changes element at current index with given parameter
     * @param element New data
     * @return Old data
     */
    E setLast(E element);
}

public interface Queue<E> extends Collection<E>
{
    /**
     * Returns an Iterator of this Queue 
     */
    Iterator iterator();
    
    /**
     * Inserts given element into this Queue 
     * @param e 
     */    
    void add(E e);

    /**
     * Adds all of the elements in the given Queue into this Collection 
     * @param c 
     */
    void addAll(Collection c);

    /**
     * Removes all the elements from this Queue
     */
    void clear(); 

    /**
     * Returns true, if the given element is in this Queue. Else returns false.
     * @param e 
     */
    boolean contains(E e);

    /**
     * Returns true, if the given Collection's elements are all exist in Queue. Else returns false.
     * @param c 
     */
    boolean containsAll(Collection c);

    /**
     * Returns true, if this Queue doesn't contain any element.
     */
    boolean isEmpty(); 

    /**
     * If it exists, removes given element from this Queue.
     * @param e 
     */
    void remove(E e); 

    /**
     * Removes every element from this Queue that is also exists in the given Collection.
     * @param c 
     */
    void removeAll(Collection c); 

    /**
     * Removes every element from this object which doesn't exist in the given Collection.
     * @param c 
     */
    void retainAll(Collection c);

    /**
     * Returns size of this Queue
     */
    int size(); 

    /**
     * Returns the element at the head of this Queue.
     * @return
     * @throws ElementNotExists
     */
    E element() throws ElementNotExists;

    /**
     * Inserts given element into this Queue.
     * @param e
     */
    void offer(E e);
    
    /**
     * Returns element at the head of this Queue. Also removes it.
     * @return
     * @throws ElementNotExists
     */
    E poll() throws ElementNotExists;
}

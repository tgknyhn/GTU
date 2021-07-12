public interface Collection<E>
{

    /**
     * Returns an Iterator of this Collection 
     */
    Iterator iterator();
    
    /**
     * Inserts given element into this Collection 
     * @param e 
     */    
    void add(E e);

    /**
     * Adds all of the elements in the given collection into this Collection 
     * @param c 
     */
    void addAll(Collection c);

    /**
     * Removes all the elements from this Collection
     */
    void clear(); 

    /**
     * Returns true, if the given element is in this Collection. Else returns false.
     * @param e 
     */
    boolean contains(E e);

    /**
     * Returns true, if the given Collection's elements are all exist in Collection. Else returns false.
     * @param c 
     */
    boolean containsAll(Collection c);

    /**
     * Returns true, if this Collection doesn't contain any element.
     */
    boolean isEmpty(); 

    /**
     * If it exists, removes given element from this Collection.
     * @param e 
     */
    void remove(E e); 

    /**
     * Removes every element from this Collection that is also exists in the given Collection.
     * @param c 
     */
    void removeAll(Collection c); 

    /**
     * Removes every element from this Collection which also exist in the given Collection.
     * @param c 
     */
    void retainAll(Collection c);

    /**
     * Returns size of this Collection
     */
    int size(); 
}
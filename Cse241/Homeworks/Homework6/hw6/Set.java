public interface Set<E> extends Collection<E>
{
    /**
     * Returns an Iterator of this Set 
     */
    Iterator iterator();
    
    /**
     * Inserts given element into this Set 
     * @param e 
     */    
    void add(E e);

    /**
     * Adds all of the elements in the given Set into this Collection 
     * @param c 
     */
    void addAll(Collection c);

    /**
     * Removes all the elements from this Set
     */
    void clear(); 

    /**
     * Returns true, if the given element is in this Set. Else returns false.
     * @param e 
     */
    boolean contains(E e);

    /**
     * Returns true, if the given Collection's elements are all exist in Set. Else returns false.
     * @param c 
     */
    boolean containsAll(Collection c);

    /**
     * Returns true, if this Set doesn't contain any element.
     */
    boolean isEmpty(); 

    /**
     * If it exists, removes given element from this Set.
     * @param e 
     */
    void remove(E e); 

    /**
     * Removes every element from this Set that is also exists in the given Collection.
     * @param c 
     */
    void removeAll(Collection c); 

    /**
     * Removes every element from this Set which also exist in the given Collection.
     * @param c 
     */
    void retainAll(Collection c);

    /**
     * Returns size of this Set
     */
    int size(); 
}

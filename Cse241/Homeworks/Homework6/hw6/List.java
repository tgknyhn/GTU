public interface List<E> extends Collection<E> 
{
    /**
     * Returns an Iterator of this List 
     */
    Iterator iterator();
    
    /**
     * Inserts given element into this List 
     * @param e 
     */    
    void add(E e);

    /**
     * Adds all of the elements in the given List into this Collection 
     * @param c 
     */
    void addAll(Collection c);

    /**
     * Removes all the elements from this List
     */
    void clear(); 

    /**
     * Returns true, if the given element is in this List. Else returns false.
     * @param e 
     */
    boolean contains(E e);

    /**
     * Returns true, if the given Collection's elements are all exist in List. Else returns false.
     * @param c 
     */
    boolean containsAll(Collection c);

    /**
     * Returns true, if this List doesn't contain any element.
     */
    boolean isEmpty(); 

    /**
     * If it exists, removes given element from this List.
     * @param e 
     */
    void remove(E e); 

    /**
     * Removes every element from this List that is also exists in the given Collection.
     * @param c 
     */
    void removeAll(Collection c); 

    /**
     * Removes every element from this object which doesn't exist in the given Collection.
     * @param c 
     */
    void retainAll(Collection c);

    /**
     * Returns size of this List
     */
    int size(); 

    /**
     * Inserts given element into given index position.
     */
    void add(int index, E e);
    
    /**
     * Changes given element's location into given index position.
     */
    void set(int index, E e);

    /**
     * Returns the element at the given index position. 
     */
    E get(int index);

    /**
     * Returns the index of the given element.
     */
    int indexOf(E e);
}

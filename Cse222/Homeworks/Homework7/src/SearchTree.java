/**
 * Determines necessary methods while searching through tree data structure
 * @param <E> Variable type that tree holds
 */
public interface SearchTree<E extends Comparable<E>> {
    /**
     * Adds given item to the tree
     * @param item Item that will be added to the tree
     * @return True when add operation is successful
     */
    boolean add(E item);

    /**
     * Returns true when given target is present in the tree data structure
     * @param target Target item that will be searched in tree
     * @return True when given target is present in the tree
     */
    boolean contains(E target);

    /**
     * Returns given item target is present in the tree data structure
     * @param target Target item that will be searched in tree
     * @return Null when given target item doesn't exist in the tree
     */
    E find(E target);

    /**
     * Removes given target item from tree
     * @param target Item that will be deleted from the tree
     * @return Value of the removed item
     */
    E delete(E target);

    /**
     * Removes given target item from tree
     * @param target Item that will be deleted from the tree
     * @return True if remove operation is successful
     */
    boolean remove(E target);
}

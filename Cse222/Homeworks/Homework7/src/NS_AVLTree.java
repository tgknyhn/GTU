import java.util.*;

/**
 * Implements NavigableSet for selected methods. Uses AVL Tree to implement methods.
 * @param <E> Data type that NavigableSet holds
 */
public class NS_AVLTree<E extends Comparable<E>> implements NavigableSet<E> {
    // Skip-List Object
    private final AVLTree<E> avlTree = new AVLTree<>();

    // Implemented
    @Override
    public boolean add(E e) {
        if(avlTree.size() == 0) {
            avlTree.add(e);
            return true;
        }
        else if(!avlTree.contains(e)) {
            avlTree.add(e);
            return true;
        }
        return false;
    }

    @Override
    public Iterator<E> iterator() {
        ArrayList<E> dataList = new ArrayList<>();

        InOrderAVLTreeTraversal(avlTree.root, dataList);

        return dataList.iterator();
    }


    @Override
    public NavigableSet<E> headSet(E toElement, boolean inclusive) {
        NavigableSet<E> set = new NS_AVLTree<>();

        InOrderAVLTreeTraversal_head(toElement, avlTree.root, set);

        if(inclusive)
            set.add(toElement);

        return set;
    }

    @Override
    public SortedSet<E> headSet(E toElement) {
        NavigableSet<E> set = new NS_AVLTree<>();

        InOrderAVLTreeTraversal_head(toElement, avlTree.root, set);

        return set;
    }

    @Override
    public NavigableSet<E> tailSet(E fromElement, boolean inclusive) {
        NavigableSet<E> set = new NS_AVLTree<>();

        InOrderAVLTreeTraversal_tail(fromElement, avlTree.root, set);

        if(inclusive)
            set.add(fromElement);

        return set;
    }

    @Override
    public SortedSet<E> tailSet(E fromElement) {
        NavigableSet<E> set = new NS_AVLTree<>();

        InOrderAVLTreeTraversal_tail(fromElement, avlTree.root, set);

        return set;
    }


    // Helper Methods

    private void InOrderAVLTreeTraversal(BinaryTree.Node<E> node, ArrayList<E> data) {
        if(node == null)
            return;

        InOrderAVLTreeTraversal(node.left, data);
        data.add(node.data);
        InOrderAVLTreeTraversal(node.right, data);
    }

    private void InOrderAVLTreeTraversal_head(E toElement, BinaryTree.Node<E> node, NavigableSet<E> set) {
        if(node == null)
            return;

        InOrderAVLTreeTraversal_head(toElement, node.left, set);
        if(toElement.compareTo(node.data) > 0)
            set.add(node.data);
        InOrderAVLTreeTraversal_head(toElement, node.right, set);
    }

    private void InOrderAVLTreeTraversal_tail(E toElement, BinaryTree.Node<E> node, NavigableSet<E> set) {
        if(node == null)
            return;

        InOrderAVLTreeTraversal_tail(toElement, node.left, set);
        if(toElement.compareTo(node.data) < 0)
            set.add(node.data);
        InOrderAVLTreeTraversal_tail(toElement, node.right, set);
    }

    // Not Implemented

    @Override
    public E lower(E e) { return null; }

    @Override
    public E floor(E e) { return null; }

    @Override
    public E ceiling(E e) { return null; }

    @Override
    public E higher(E e) { return null; }

    @Override
    public E pollFirst() { return null; }

    @Override
    public E pollLast() { return null; }

    @Override
    public Iterator<E> descendingIterator() { return null; }

    @Override
    public NavigableSet<E> descendingSet() { return null; }

    @Override
    public NavigableSet<E> subSet(E fromElement, boolean fromInclusive, E toElement, boolean toInclusive) { return null; }

    @Override
    public SortedSet<E> subSet(E fromElement, E toElement) { return null; }

    @Override
    public Comparator<? super E> comparator() { return null; }

    @Override
    public E first() { return null; }

    @Override
    public E last() { return null; }

    @Override
    public int size() { return 0; }

    @Override
    public boolean remove(Object o) { return false; }

    @Override
    public boolean isEmpty() { return false; }

    @Override
    public boolean contains(Object o) { return false; }

    @Override
    public Object[] toArray() { return new Object[0]; }

    @Override
    public <T> T[] toArray(T[] a) { return null; }

    @Override
    public boolean containsAll(Collection<?> c) { return false; }

    @Override
    public boolean addAll(Collection<? extends E> c) { return false; }

    @Override
    public boolean retainAll(Collection<?> c) { return false; }

    @Override
    public boolean removeAll(Collection<?> c) { return false; }

    @Override
    public void clear() { }

}

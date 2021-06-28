import java.util.*;

/**
 * Implements NavigableSet for selected methods. Uses SkipList to implement methods.
 * @param <E> Data type that NavigableSet holds
 */
public class NS_SkipList<E extends Comparable<E>> implements NavigableSet<E> {
    // Skip-List Object
    private final SkipList<E> skipList = new SkipList<>();

    // Implemented
    @Override
    public boolean add(E e) {
        if(skipList.size() == 0) {
            skipList.add(e);
            return true;
        }
        else if(!skipList.contains(e)) {
            skipList.add(e);
            return true;
        }
        return false;
    }

    @Override
    public boolean remove(Object o) {
        if(skipList.size() == 0)
            return false;
        else if(!skipList.contains((E)o))
            return false;

        return skipList.remove((E)o) != null;
    }

    @Override
    public Iterator<E> descendingIterator() {
        ArrayList<E> data = new ArrayList<>();

        // inserting elements
        for(E e : skipList)
            data.add(e);

        // reversing it
        Collections.reverse(data);

        return data.iterator();
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
    public Iterator<E> iterator() { return null; }

    @Override
    public NavigableSet<E> descendingSet() { return null; }

    @Override
    public NavigableSet<E> subSet(E fromElement, boolean fromInclusive, E toElement, boolean toInclusive) { return null; }

    @Override
    public NavigableSet<E> headSet(E toElement, boolean inclusive) { return null; }

    @Override
    public NavigableSet<E> tailSet(E fromElement, boolean inclusive) { return null; }

    @Override
    public SortedSet<E> subSet(E fromElement, E toElement) { return null; }

    @Override
    public SortedSet<E> headSet(E toElement) { return null; }

    @Override
    public SortedSet<E> tailSet(E fromElement) { return null; }

    @Override
    public Comparator<? super E> comparator() { return null; }

    @Override
    public E first() { return null; }

    @Override
    public E last() { return null; }

    @Override
    public int size() { return 0; }

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
import java.util.Arrays;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.Random;

/**
 * SkipList is improved version of LinkedList data structure. Implements Iterable interface
 * Let's insertion and remove operation done with O(logn) time complexity.
 * @param <E> Data type that SkipList holds. It must extend comparable class.
 */
public class SkipList<E extends Comparable<E>> implements Iterable<E>{
    // Data Fields
    private static final double LOG2 = Math.log(2.0);
    private int maxLevel = 4;
    private int maxCap = (int) (Math.pow(2, maxLevel) - 1);
    private int size = 0;
    private SLNode<E> head;


    // Constructor //

    /**
     * Initializes SkipList object
     */
    public SkipList() { head = new SLNode<>(maxLevel, null); }

    /**
     * Adds given item to the SkipList
     * @param item Item that will be inserted to the list
     */
    public void add(E item){
        int level = logRandom();
        SLNode<E>[] pred = search(item);
        SLNode<E> newNode = new SLNode<>(level, item);
        for(int i = 0; i < level; i++){
            newNode.links[i] = pred[i].links[i];
            pred[i].links[i] = newNode;
        }
        size++;
        if (size > maxCap) {
            maxLevel++;
            maxCap = (int) (Math.pow(2, maxLevel) - 1);
            head.links = Arrays.copyOf(head.links, maxLevel);
            pred = Arrays.copyOf(head.links, maxLevel);
            pred[maxLevel-1] = head;
        }
    }

    /**
     * Returns given parameter if it exist in the SkipList data structure
     * @param target Item that will be searched in the SkipList
     * @return Null when given item doesn't exist in the SkipList
     */
    public E find(E target) {
        SLNode<E>[] pred = search(target);

        if (pred[0].links[0] != null && pred[0].links[0].data.compareTo(target) == 0)
            return pred[0].links[0].data;
        else
            return null;
    }

    /**
     * Removes given parameter from the SkipList if it exist
     * @param target Item that will be removed from the SkipList
     * @return Removed item. Null when given item doesn't exist in the SkipList
     */
    public E remove(E target){
        SLNode<E>[] pred = search(target);


        if(pred[0].links[0].data.compareTo(target) == 0){
            // getting the value of removed item
            E removed = pred[0].links[0].data;
            // decreasing the size
            size--;

            try {
                for(int i=0; i<pred[0].links[0].links.length; i++)
                    pred[i].links[i] = pred[i].links[i].links[i];
            } catch (NullPointerException exception) {
                return null;
            }

            return removed;
        }
        else
            return null;
    }


    @SuppressWarnings("unchecked")
    private SLNode<E>[] search (E target) {
        SLNode<E>[] pred = (SLNode<E>[]) new SLNode[maxLevel]; SLNode<E> current = head;
        for (int i = current.links.length - 1; i >= 0; i--) {
            while (current.links[i] != null && current.links[i].data.compareTo(target) < 0)
                current = current.links[i];

            pred[i] = current;
        }
        return pred;
    }

    /**
     * Returns the size of the data
     * @return Size of the data
     */
    public int size(){ return size; }

    /**
     * Returns true when given target exist in the SkipList data structure
     * @param target Target that will be searched in the SkipList data structure
     * @return True when item exist. False when doesn't exist.
     */
    public boolean contains(E target){ return find(target) != null; }

    private int logRandom(){
        Random rand = new Random();
        int r = rand.nextInt(maxCap);
        int k = (int) (Math.log(r + 1) / LOG2);
        if (k > maxLevel - 1) {
            k = maxLevel - 1;
        }
        return maxLevel - k;
    }

    /**
     * Returns an iterator over elements of type {@code T}.
     *
     * @return an Iterator.
     */
    public Iterator<E> iterator() { return new SkipListIter(); }



    // INNER CLASS - Node //

    @SuppressWarnings("unchecked")
    private static class SLNode<E>{
        SLNode<E>[] links;
        E data;
        public SLNode(int m, E data) {
            links = (SLNode<E>[]) new SLNode[m];
            this.data = data;
        }
    }



    // INNER CLASS - Iterator //

    private class SkipListIter implements Iterator<E>{
        private SLNode<E> current = head;
        /**
         * Returns {@code true} if the iteration has more elements.
         * (In other words, returns {@code true} if {@link #next} would
         * return an element rather than throwing an exception.)
         *
         * @return {@code true} if the iteration has more elements
         */
        @Override
        public boolean hasNext() {
            return current.links[0] != null;
        }

        /**
         * Returns the next element in the iteration.
         *
         * @return the next element in the iteration
         * @throws NoSuchElementException if the iteration has no more elements
         */
        @Override
        public E next() {
            if(!hasNext()){
                throw new NoSuchElementException();
            }
            return (current = current.links[0]).data;
        }
    }
}

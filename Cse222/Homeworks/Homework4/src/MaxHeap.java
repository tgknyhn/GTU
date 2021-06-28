import java.util.ArrayList;

/**
 * Class that implements methods for Max Heap data structure
 * @param <E> Data Type
 */
public class MaxHeap<E extends Comparable<E>> {
    /**
     * Shows maximum size that data arr can hold
     */
    private final int MAX_SIZE;

    /**
     * Array that holds heap data
     */
    private ArrayList<E> data;

    /**
     * Array that holds number occurrences
     */
    private ArrayList<Integer> occurrences;

    /**
     * Shows current size of Max Heap
     */
    private int size;

    /**
     * Initializes MinHeap class.
     */
    public MaxHeap() { this(7); }

    /**
     * Initializes MinHeap class with given parameters
     * @param maxSize Max possible element that MaxHeap can contain
     */
    public MaxHeap(int maxSize) {
        // giving a max size for MaxHeap
        MAX_SIZE = maxSize;
        // initializing data and occurrences
        data = new ArrayList<>(MAX_SIZE);
        occurrences = new ArrayList<>(MAX_SIZE);
    }

    /**
     * Swaps two data with given index in the array
     * @param index1 First data's index
     * @param index2 Second data's index
     */
    private void swap(int index1, int index2) {
        // Backup first data and occurrence by using temp value
        E   tempData       = null;
        int tempOccurrence = 0;

        try {
            tempData       = data.get(index1);
            tempOccurrence = occurrences.get(index1);
        } catch (IndexOutOfBoundsException e) {
            System.out.println("index: " + index1 + "size: " + data.size());
        }

        // Inserting second data and occurrence into first index
        data.set(index1, data.get(index2));
        occurrences.set(index1, occurrences.get(index2));

        // Inserting first data and occurrence into second index by using temp
        data.set(index2, tempData);
        occurrences.set(index2, tempOccurrence);
    }

    /**
     * Returns index of parent of given node
     * @param index Index of the node
     * @return Parent's index of the node
     */
    private int parent(int index) { return (index / 2 == 0) ? 1 : (index / 2); }

    /**
     * Returns index of left child of given node
     * @param index Index of the node
     * @return Left child's index of the node
     */
    private int left(int index) { return 2 * index; }

    /**
     * Returns index of right child of given node
     * @param index Index of the node
     * @return Right child's index of the node
     */
    private int right(int index) { return (2 * index) + 1; }

    /**
     * Returns true if selected node is a leaf(has no children) node
     * @param index Selected node
     * @return True when selected node is a leaf node
     */
    private boolean isLeaf(int index) { return index >= (size / 2) && index <= size; }

    /**
     * Swaps node with it's one of the children if they break the rule of max heap data structure
     * @param index Index of selected node
     */
    private void maxHeapify(int index) {
        // temp data ** nodes **
        E node, leftChild, rightChild;

        try { node        = data.get(index); }        catch (IndexOutOfBoundsException i) { node = getMin(); }
        try { leftChild   = data.get(left(index));  } catch (IndexOutOfBoundsException i) { leftChild  = getMax(); }
        try { rightChild  = data.get(right(index)); } catch (IndexOutOfBoundsException i) { rightChild = getMax(); }


        // check if given index is leaf node
        if(!isLeaf(index)) {
            // check if given node's children are bigger than their parent
            if( (node.compareTo(leftChild) < 0) || (node.compareTo(rightChild) < 0) ) {
                // if first one is bigger swap with it's parent and call maxHeapify with that index
                if(leftChild.compareTo(rightChild) > 0) {
                    swap(index, left(index));
                    maxHeapify(left(index));
                }
                else {
                    swap(index, right(index));
                    maxHeapify(right(index));
                }
            }
        }
    }

    /**
     * Swaps all data in MaxHeap class according to Max Heap data structure's rule
     */
    public void builtMaxHeap() {
        for(int i = (size/2); i > 0; i--)
            maxHeapify(i);
    }

    /**
     * Returns root element
     * @return Root element of Max Heap
     */
    public E getMax() { return data.get(1); }

    /**
     * Changes Max value of the heap with given parameter
     * @param item New value of max
     */
    public void setMax(E item) { data.set(1, item); }

    /**
     * Returns least important element
     * @return Least important element of Max Heap
     */
    public E getMin() { return data.get(size); }

    /**
     * Returns size of the current MaxHeap tree
     * @return Size of the current MaxHeap tree
     */
    public int getSize() { return size; }

    /**
     * Returns occurrence number of given index element
     * @param element That which occurrence returned
     * @return Number of occurrences of given parameter
     */
    public int getOccurrence(E element) {
        int number;

        try {
            number = occurrences.get(searchElement(element));
        } catch (IndexOutOfBoundsException exception) {
            number = 0;
        }

        return number;
    }

    /**
     * Inserts given element into the Max Heap
     * @param e Element that will be inserted into the Max Heap
     */
    public int insert(E e) {
        // index of given element if it already exist
        int index;

        // when heap is empty
        if(size == 0) {
            data.add(e);
            occurrences.add(1);
        }
        // when given element already exist in the heap
        else if(searchElement(e) != -1) {
            index = searchElement(e);
            occurrences.set(index, occurrences.get(index) + 1);
            return occurrences.get(index);
        }
        // if heap is full
        else if(size == MAX_SIZE) {
            System.out.println("You are trying to add more than you could. (Max: " + MAX_SIZE + ") (Value which not added: " + e + ")");
            return 0;
        }

        // inserting data and it's occurence
        data.add(e);
        occurrences.add(1);
        // increasing current size of data array
        size++;
        // getting temp size
        int tmpSize = size;

        while( data.get(tmpSize).compareTo(data.get(parent(tmpSize))) > 0 ) {
            swap(tmpSize, parent(tmpSize));
            tmpSize = parent(tmpSize);
        }

        return 1;
    }

    /**
     * Search given element in the Max Heap. Returns element's index if it is present. (-1 when element does not exist)
     * @param e Element that method looks for
     * @return Index of that element. Returns -1 when element does not exist.
     */
    public int searchElement(E e) {
        // searching through every element in the heap
        for(int i=1; i<=size; i++)
            if(e.compareTo(data.get(i)) == 0)
                return i;

        return -1;
    }

    /**
     * Inserts every data in given parameter into Max Heap
     * @param secondHeap Inserted data
     */
    public void merge(MaxHeap<E> secondHeap) {
        E   data;
        int index;

        for(int i=1; i<=secondHeap.size; i++) {
            data  = secondHeap.data.get(i);
            index = searchElement(data);

            if(index != -1)
                occurrences.set(index, occurrences.get(index) + secondHeap.occurrences.get(i));
            else
                this.insert(secondHeap.data.get(i));
        }
    }

    /**
     * Removes an element from Max Heap data structure according element's priority.
     * @param priority Priority of the element. Max Priority = 1, Min Priority = array size
     * @return Removed element
     */
    public E removeWithPriority(int priority) {
        int lastIndex = size;
        E parameter = data.get(priority);
        E minData   = getMin();

        // if number of occurrences of given data is not 1, then reduce it by 1
        if(occurrences.get(priority) > 1) {
            occurrences.set(priority, occurrences.get(priority) - 1);
            return parameter;
        }

        // swapping least important element with given element if it is not least important element
        if(parameter.compareTo(minData) != 0)
            swap(priority, lastIndex);

        // removing last element
        data.remove(lastIndex);

        // decreasing array size by 1
        size--;

        return parameter;
    }

    /**
     * Prints current tree of Max Heap to the console
     */
    public void print() {
        if(getSize() < 2)
            System.out.println("(Empty heap)");

        StringBuilder string = new StringBuilder();

        for(int i=1; i <= size/2; i++) {
            string.append(i).append(")\tParent: ");
            try { string.append(data.get(i)); } catch (Exception a) { string.append("empty"); }
            string.append("\t\t Left Child: ");
            try { string.append(data.get(left(i))); } catch (Exception a) { string.append("empty"); }
            string.append("\t  Right Child: ");
            try { string.append( data.get(right(i))); } catch (Exception a) { string.append("empty"); }
            string.append("\n");
        }

        System.out.println(string);
    }

    /**
     * Prints out all data in Max Heap tree.
     * @return Data
     */
    @Override
    public String toString() {
        StringBuilder string = new StringBuilder();

        if(getSize() < 2)
            string.append("Heap: empty");
        else
            string.append("Heap: ");

        for(int i=1; i <= size/2; i++) {
            string.append("(P: ");
            try { string.append(     data.get(i)    ).append(",").append(     occurrences.get(i)    ); } catch (Exception a) { string.append("empty"); }
            string.append(" L: ");
            try { string.append( data.get(left(i))  ).append(",").append( occurrences.get(left(i))  ); } catch (Exception a) { string.append("empty"); }
            string.append(" R: ");
            try { string.append( data.get(right(i)) ).append(",").append( occurrences.get(right(i)) ); } catch (Exception a) { string.append("empty"); }
            string.append(") ");
        }

        return string.toString();
    }

    /**
     * Returns an iterator for Max Heap class
     * @return Iterator for Max Heap class
     */
    public Iterator<E> iterator() { return new myIterator(); }

    /**
     * Class that implements methods in Iterator interface.
     * Also adds another method called setLast for Max Heap class
     */
    private class myIterator implements Iterator<E> {
        // Last returned element index
        private int lastItemReturnedIndex;
        // Last returned element
        private E lastItemReturned;
        // current index
        private int index;

        private myIterator() {
            lastItemReturnedIndex = -1;
            lastItemReturned = null;
            index = 1;
        }

        @Override
        public void remove() { /* empty */ }

        @Override
        public boolean hasNext() { return index <= size; }

        @Override
        public E next() {
            // assigning next item into lastItemReturned
            lastItemReturned = data.get(index);
            // assigning next item index into lastItemReturnedIndex
            lastItemReturnedIndex = index;
            // increasing index by one
            index++;

            return lastItemReturned;
        }

        /**
         * Changes the element which lastItemReturnedIndex points into with given parameter
         * @param element New data
         * @return Changed element
         * @throws NullPointerException When lastItemReturned is null
         */
        public E setLast(E element) throws NullPointerException {
            if(lastItemReturned == null)
                throw new NullPointerException();

            // changing value of last returned element
            data.set(lastItemReturnedIndex, element);


            int currIndex = lastItemReturnedIndex;
            // if it is bigger than parent node then swap it until it is not
            if(data.get(currIndex).compareTo(data.get(parent(currIndex))) > 0) {
                while( data.get(currIndex).compareTo(data.get(parent(currIndex))) > 0 ) {
                    swap(currIndex, parent(currIndex));
                    currIndex = parent(currIndex);
                }
            }

            builtMaxHeap();

            return lastItemReturned;
        }
    }
}

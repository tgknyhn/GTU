import java.util.Iterator;

/**
 * An hashtable class which implement KWHashMap interface. Uses Open Addressing technique.
 * @param <K> Key
 * @param <V> Value
 */
public class HashtableOpen<K,V> implements  KWHashMap<K,V> {
    // Capacity
    private static final int START_CAPACITY = 101;
    // Key - Values
    private       Entry<K,V>[] table;
    private final Entry<K,V> DELETED = new Entry<>(null, null);

    // Data fields
    private double LOAD_THRESHOLD = 0.75;
    private int numKeys;
    private int numDeletes;

    /**
     * Initializes HashtableOpen object
     */
    public HashtableOpen() {
        //noinspection unchecked
        table = new Entry[START_CAPACITY];
    }

    // ** [Public Methods] **

    @Override
    public V get(Object key) {
        // getting index of the key
        int index = find(key);

        if(table[index] != null)
            return table[index].getValue();
        else
            return null;
    }

    @Override
    public V put(K key, V value) {
        // getting index of the key
        int index = find(key);

        // if index is empty than add new entry
        if(table[index] == null) {
            // adding new key & value
            table[index] = new Entry<>(key, value);
            numKeys++;

            // checking if rehash is needed or not
            double loadFactor = (double) (numKeys + numDeletes) / table.length;

            if(loadFactor > LOAD_THRESHOLD)
                rehash();

            return null;
        }

        V oldValue = table[index].getValue();

        table[index].setValue(value);

        return oldValue;
    }

    @Override
    public V remove(Object key) {
        // getting index of the key
        int index = find(key);

        // if table is null then return null
        if(table[index] == null)
            return null;

        // old value
        V oldValue = table[index].getValue();

        // setting it into DELETE entry
        table[index] = DELETED;

        numDeletes++;
        numKeys--;

        return oldValue;
    }

    @Override
    public int size() {
        return numKeys;
    }

    @Override
    public boolean isEmpty() {
        return numKeys < 1;
    }

    /**
     * Returns an MapIterator object starting from given key
     * @param key Start point for iterator
     * @return MapIterator object
     */
    public MapIterator iterator(K key) { return new MapIterator(key); }

    /**
     * Returns an MapIterator objecet
     * @return MapIterator object
     */
    public MapIterator iterator() { return new MapIterator(); }

    // ***********************

    // ** [Private Methods] **

    private int find(Object key) {
        // Starting index
        int index = key.hashCode() % table.length;
        // making index positive
        if(index < 0)
            index += table.length;

        // increment index until an empty slot is reached or the key is found
        while( (table[index] != null) && (!key.equals(table[index].getKey())) ) {
            index++;

            if(index >= table.length)
                index = 0;
        }

        return index;
    }

    @SuppressWarnings("unchecked")
    private void rehash() {
        // reference to oldTable
        Entry<K,V>[] oldTable = table;
        // double the capacity
        table = new Entry[(oldTable.length+1)*2];

        // insert values
        numKeys = 0;
        numDeletes = 0;
        for(int i=0; i<oldTable.length; i++)
            if( (oldTable[i] != null) && (oldTable[i] != DELETED) )
                put(oldTable[i].getKey(), oldTable[i].getValue());
    }


    // ** [Inner Classes] **

    private static class Entry<K,V> {
        private final K key;
        private V value;

        public Entry(K key, V value) {
            this.key = key;
            this.value = value;
        }

        public K getKey() {
            return key;
        }

        public V getValue() {
            return value;
        }

        public V setValue(V value) {
            V oldValue = this.value;
            this.value = value;
            return oldValue;
        }
    }

    /**
     * An iterator class that implements Iterator interface for the HashtableOpen class.
     * Additionaly have prev() method
     */
    public class MapIterator implements Iterator<K> {
        private K lastKeyReturned = null;
        // shows key which iterator points into
        private int currentIndex = 1;
        // shows how many time MapIterator class iterated
        private int currentIteration = 0;

        /**
         * Initializes the MapIterator class with given parameter
         * @param key Starting key
         */
        public MapIterator(K key) {
            // finding index of given key
            int index = find(key);
            // starting from given key if key exist
            if(table[index] != null)
                currentIndex = index;
        }

        /**
         * Initializes the MapIterator class starting from first key
         */
        public MapIterator() { /* empty */ }

        @Override
        public boolean hasNext() {
            return (currentIteration < size());
        }

        @Override
        public K next() {
            // getting not null current key
            while(table[currentIndex] == null)
                currentIndex = (currentIndex + 1) % START_CAPACITY;


            lastKeyReturned = table[currentIndex].getKey();
            // incrementing iteration and index size
            currentIteration++;
            currentIndex = (currentIndex + 1) % START_CAPACITY;
            // returning lastKeyReturned
            return lastKeyReturned;
        }

        /**
         * Points into previous key and returns it
         * @return Previous key
         */
        public K prev() {
            // getting not null current key
            while(table[currentIndex] == null)
                currentIndex = (currentIndex + START_CAPACITY - 1) % START_CAPACITY;

            lastKeyReturned = table[currentIndex].getKey();
            // decrementing iteration and index size
            currentIteration--;
            currentIndex = (currentIndex + START_CAPACITY - 1) % START_CAPACITY;
            // returning lastKeyReturned
            return lastKeyReturned;
        }
    }
}
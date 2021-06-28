import java.util.TreeSet;

/**
 * An hashtable class which implement KWHashMap interface. Uses chaining technique and TreeSet data structure for key-value parities.
 * @param <K> Key
 * @param <V> Value
 */
public class HashtableTree<K extends Comparable<K>,V> implements KWHashMap<K,V> {
    private static final int    CAPACITY = 101;
    private static final double LOAD_THRESHOLD = 3.0;
    // Key - Values
    private TreeSet<Entry<K,V>>[] table;
    // size
    private int numKeys;

    /**
     * Initializes HashtableTree object with initial CAPACITY
     */
    @SuppressWarnings("unchecked")
    public HashtableTree() {
        table = new TreeSet[CAPACITY];
    }

    @Override
    public V get(Object key) {
        // find key index
        int index = key.hashCode() % table.length;

        if(index < 0)
            index += table.length;
        // if key doesn't exist
        if(table[index] == null)
            return null;

        // search through list to find key
        for(HashtableTree.Entry<K,V> nextItem : table[index])
            if(nextItem.getKey().equals(key))
                return nextItem.getValue();

        return null;
    }

    @Override
    public V put(K key, V value) {
        // find key index
        int index = key.hashCode() % table.length;

        if(index < 0)
            index += table.length;
        if(table[index] == null)
            table[index] = new TreeSet<>();

        for(HashtableTree.Entry<K,V> nextItem : table[index]) {
            if(nextItem.getKey().equals(key)) {
                V oldValue = nextItem.getValue();
                nextItem.setValue(value);
                return oldValue;
            }
        }


        table[index].add(new Entry<>(key, value));
        numKeys++;
        if(numKeys > (LOAD_THRESHOLD * table.length))
            rehash();

        return null;
    }

    @Override
    public V remove(Object key) {
        // find key index
        int index = key.hashCode() % table.length;

        if(index < 0)
            index += table.length;

        try {
            for(Entry<K,V> nextItem : table[index]) {
                if(nextItem.getKey().equals(key)) {
                    table[index].remove(nextItem);
                    if(table[index].size() == 0)
                        table[index] = null;

                    numKeys--;
                    return nextItem.getValue();
                }
            }
        } catch (NullPointerException n) {
            throw new NullPointerException();
        }


        return null;
    }

    @Override
    public int size() {
        return numKeys;
    }

    @Override
    public boolean isEmpty() {
        return numKeys < 1;
    }

    @Override
    public String toString() {
        StringBuilder stringB = new StringBuilder();

        for(int i=0, j=1; i<CAPACITY; i++)
            if(table[i] != null) {
                stringB.append("\n").append(j).append(") ").append("Key: ").append(String.format("%5s", table[i].first().getKey())).append(" \tValue: ").append(table[i].first().getValue());
                j++;
            }

        return stringB.toString();
    }

    // ** [Private Methods] **

    @SuppressWarnings("unchecked")
    private void rehash() {
        // reference to oldTable
        TreeSet<HashtableTree.Entry<K,V>>[] oldTable = table;
        // double the capacity
        table = new TreeSet[(oldTable.length + 1) * 2];

        numKeys = 0;
        // inserting values
        for(int i=0; i<oldTable.length; i++) {
            try {
                if(oldTable[i].first() != null)
                    put(oldTable[i].first().getKey(), oldTable[i].first().getValue());
            } catch (NullPointerException ignore) { }
        }
    }

    // ** [Inner Classes] **

    private static class Entry<K extends Comparable<K>,V> implements Comparable<K>{
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

        @Override
        public int compareTo(K o) {
            if(o.compareTo(getKey()) > 0)
                return -1;
            else if(o.compareTo(getKey()) < 0)
                return 1;
            else
                return 0;
        }
    }
}

import java.util.Arrays;
import java.util.LinkedList;

/**
 * An hashtable class which implement KWHashMap interface. Uses coalesced hashing technique.
 * @param <K> Key
 * @param <V> Value
 */
public class HashtableCoalesced<K, V> implements KWHashMap<K,V> {
    // Capacity
    private static       int    CAPACITY = 10;
    private        final double LOAD_THRESHOLD = 0.75;
    // Key - Values
    private Node<K>[] table;
    private Node<K> DELETED = new Node<>(null, null);
    // Data Fields
    private int numKeys;

    // ** [Public Methods] **

    /**
     * Initializes HashtableCoalesced class with initial CAPACITY
     */
    public HashtableCoalesced() {
        table   = new Node[CAPACITY];
        numKeys = 0;
    }

    @Override
    public V get(Object key) {
        // getting index of the key
        int     index    = find(key);
        Node<K> currNode;

        try {
            currNode = table[index];
        } catch (IndexOutOfBoundsException i) {
            return null;
        }

        while(currNode != null) {
            // check for equality
            if(currNode.entry.getKey().equals(key))
                return currNode.entry.getValue();
            // if next index is -1 then return null
            if(currNode.next == -1)
                return null;
            // next node
            currNode = table[currNode.next];
        }

        return null;
    }

    @Override
    public V put(K key, V value) {
        // getting index of the key
        int index = key.hashCode() % table.length;

        if(index < 0)
            index += table.length;

        Node<K> currNode   = table[index];
        Node<K> parentNode = table[index];

        int newIndex     = index;
        int searchedKeys = 0;

        while(currNode != null) {
            // Next index
            newIndex = (index + (searchedKeys * searchedKeys)) % table.length;

            if(key.equals(currNode.entry.getKey()))
                return null;
            else if(newIndex < 0)
                newIndex += table.length;
            else if(searchedKeys > table.length*500)
                return null;

            // next node
            if( (key.hashCode() % table.length ) == (currNode.entry.getKey().hashCode() % table.length) )
                parentNode = currNode;

            currNode = table[newIndex];
            // +1 searched key
            searchedKeys++;
        }

        if( (parentNode != currNode) && parentNode.next == -1)
            parentNode.next = newIndex;

        table[newIndex] = new Node<>(key, value);
        numKeys++;

        double loadFactor = (double) numKeys / table.length;

        if(loadFactor > LOAD_THRESHOLD)
            rehash();

        return null;
    }

    @Override
    public V remove(Object key) throws NullPointerException {
        // get index of the key
        int delete      = 0;
        int index       = find(key);
        int parentIndex = findParent(key);
        V   oldValue    = null;

        if(index == -1)
            throw new NullPointerException();
        else if(parentIndex == -1 && table[index].next == -1)
            table[index] = DELETED;
        else if(table[index].next != -1) {
            delete = table[index].next;

            Node<K> nextNode = table[table[index].next];
            oldValue         = table[index].entry.getValue();

            table[index].entry = new Entry<>(nextNode.entry.getKey(), nextNode.entry.getValue());
            table[index].next  = nextNode.next;

            table[delete] = DELETED;
        }
        else
            table[index] = DELETED;

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

    @Override
    public String toString() {
        StringBuilder string = new StringBuilder();
        K key;
        V value;
        int next;

        // Header
        string.append("\tIndex").append("\t Key").append("\tNext").append("\tValue\n");
        string.append("\t-----").append("\t-----").append("\t----").append("\t-----\n");
        // Data
        for(int i=0; i<table.length; i++) {
            try {
                key   = table[i].entry.getKey();
                value = table[i].entry.getValue();
                next  = table[i].next;
            } catch (Exception e) {
                key   = null;
                value = null;
                next  = -1;
            }
            if(key == null) {
                // index
                string.append("\t  ").append(i).append("  ");
                // key
                string.append("  ").append(String.format("%5s", " - "));
                // next
                string.append("   ").append(String.format("%5s", " - "));
                // value
                string.append("   ").append(String.format("%5s", "- ")).append("\n");
            }
            else {
                // index
                string.append("\t  ").append(i).append("  ");
                // key
                string.append("  ").append(String.format("%5s", key));
                // next
                if(next == -1)
                    string.append("  ").append(String.format("%5s", " -"));
                else
                    string.append("  ").append(String.format("%5s", next));
                // value
                string.append("   ").append(String.format("%5s", value)).append("\n");
            }
        }

        return string.toString();
    }

    // ** [Private Methods] **

    private int find(Object key) {
        // getting index of the object
        int index = key.hashCode() % table.length;

        if(index < 0)
            index += table.length;

        int newIndex     = index;
        int searchedKeys = 0;

        while(table[newIndex] != null && searchedKeys <= table.length) {
            //
            try {
                if(table[newIndex].entry.getKey().equals(key))
                    return newIndex;
            } catch (NullPointerException ignore) { }
            //
            newIndex = (index + (searchedKeys*searchedKeys)) % table.length;
            //
            searchedKeys++;
        }

        return -1;
    }

    private int findParent(Object key) {
        // getting index of the object
        int parentIndex = key.hashCode() % table.length;
        int index       = find(key);

        if(parentIndex < 0)
            parentIndex += table.length;

        if(index == -1)
            return -1;
        else if(parentIndex == index)
            return -2;
        else if (parentIndex != -1){
            while(table[parentIndex].next != index) {
                parentIndex = table[parentIndex].next;
                if(parentIndex == -1)
                    return parentIndex;
            }
        }
        return parentIndex;
    }

    private void rehash() {
        // reference to oldTable
        Node<K>[] oldTable = table;
        // double the capacity
        table     = new Node[(oldTable.length)*2];
        CAPACITY += table.length;

        // insert values
        numKeys = 0;
        for(int i=0; i<oldTable.length; i++)
            if( (oldTable[i] != null) && (oldTable[i] != DELETED) )
                put(oldTable[i].entry.getKey(), oldTable[i].entry.getValue());
    }

    // ** [Inner Classes] **

    // Holds entries
    private class Node<K> {
        // entry
        public Entry<K,V> entry;
        // next entry index
        public int next;

        // constructors
        public Node(K key, V value) {
            this.entry = new Entry<>(key, value);
            this.next = -1;
        }
    }

    // Holds key&value
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
    }
}


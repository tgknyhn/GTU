/**
 * An HashMap interface which contains some methods of HashMap Data Structure
 * @param <K> Key
 * @param <V> Value
 */
public interface KWHashMap<K,V> {
    /**
     * Returns value of the given key. Returns null if key doesn't exist in the HashMap
     * @param key Key that will be searched in the method
     * @return Equivalent value of given key
     */
    V get(Object key);

    /**
     * Puts given key into the HashMap with it's value
     * @param key Key that will be inserted into the HashMap
     * @param value Value equivalent of the key
     * @return return old value of the given key if given key already exist
     */
    V put(K key, V value);

    /**
     * Removes given key from HashMap. Returns null if given key doesn't exist in the HashMap
     * @param key Key that will be removed from the HashMap
     * @return Value equivalent of the key. Null if key doesn't exist.
     */
    V remove(Object key);

    /**
     * Returns number of keys that is available in the HashMap
     * @return Total number of key / values
     */
    int size();

    /**
     * Returns true when there is no key / value parity in the HashMap
     * @return True when there is no key / value parity in the HashMap.
     */
    boolean isEmpty();

}
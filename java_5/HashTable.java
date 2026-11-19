import java.util.ArrayList;
import java.util.List;

public final class HashTable<T extends Hashable<K>, K> {
    private final List<T>[] buckets;
    private final HashFunction<K> hashFunction;
    private int size;

    @SuppressWarnings("unchecked")
    public HashTable(int capacity, HashFunction<K> hashFunction) {
        if (capacity <= 0) {
            throw new IllegalArgumentException("capacity must be positive");
        }
        if (hashFunction == null || hashFunction.getTableSize() != capacity) {
            throw new IllegalArgumentException("hash function size must match table capacity");
        }
        this.hashFunction = hashFunction;
        buckets = (List<T>[]) new List<?>[capacity];
        for (int i = 0; i < capacity; i++) {
            buckets[i] = new ArrayList<>();
        }
    }

    public void add(K key, T value) {
        if (value == null) {
            throw new IllegalArgumentException("value must not be null");
        }
        if (!value.contains(key)) {
            throw new IllegalArgumentException("value does not contain the supplied key");
        }
        buckets[hashFunction.indexFor(key)].add(value);
        size++;
    }

    public List<T> find(K key) {
        List<T> matches = new ArrayList<>();
        for (T value : buckets[hashFunction.indexFor(key)]) {
            if (value.contains(key)) {
                matches.add(value);
            }
        }
        return matches;
    }

    public T findFirst(K key) {
        for (T value : buckets[hashFunction.indexFor(key)]) {
            if (value.contains(key)) {
                return value;
            }
        }
        return null;
    }

    public int remove(K key) {
        List<T> bucket = buckets[hashFunction.indexFor(key)];
        int removed = 0;
        for (int i = bucket.size() - 1; i >= 0; i--) {
            if (bucket.get(i).contains(key)) {
                bucket.remove(i);
                removed++;
            }
        }
        size -= removed;
        return removed;
    }

    public int size() {
        return size;
    }

    public int capacity() {
        return buckets.length;
    }

}

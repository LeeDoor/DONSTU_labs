public abstract class HashFunction<K> {
    private final int tableSize;

    protected HashFunction(int tableSize) {
        if (tableSize <= 0) {
            throw new IllegalArgumentException("tableSize must be positive");
        }
        this.tableSize = tableSize;
    }

    public final int getTableSize() {
        return tableSize;
    }

    public abstract int hash(K key);

    public final int indexFor(K key) {
        if (key == null) {
            throw new IllegalArgumentException("key must not be null");
        }
        return Math.floorMod(hash(key), tableSize);
    }
}

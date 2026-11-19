public final class GPairBag<T, U> {
    private final PairBag pairs;

    public GPairBag(int capacity) {
        pairs = new PairBag(capacity);
    }

    public void add(Pair<T, U> pair) {
        pairs.add(pair);
    }

    public Pair<T, U> remove() {
        return cast(pairs.remove());
    }

    public Pair<T, U> get() {
        return cast(pairs.get());
    }

    public int size() {
        return pairs.size();
    }

    public boolean isEmpty() {
        return pairs.isEmpty();
    }

    @SuppressWarnings("unchecked")
    private Pair<T, U> cast(Pair<?, ?> pair) {
        return (Pair<T, U>) pair;
    }
}

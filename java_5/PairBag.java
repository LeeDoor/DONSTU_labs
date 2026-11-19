public final class PairBag {
    private final Bag bag;

    public PairBag(int capacity) {
        bag = new Bag(capacity);
    }

    public void add(Pair<?, ?> pair) {
        if (pair == null) {
            throw new IllegalArgumentException("pair must not be null");
        }
        bag.add(pair);
    }

    public Pair<?, ?> remove() {
        return (Pair<?, ?>) bag.remove();
    }

    public Pair<?, ?> get() {
        return (Pair<?, ?>) bag.get();
    }

    public int size() {
        return bag.size();
    }

    public boolean isEmpty() {
        return bag.isEmpty();
    }
}

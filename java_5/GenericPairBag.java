import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ThreadLocalRandom;

public final class GenericPairBag<T, U> {
    private final List<Pair<T, U>> pairs = new ArrayList<>();

    public void add(Pair<T, U> pair) {
        if (pair == null) {
            throw new IllegalArgumentException("pair must not be null");
        }
        pairs.add(ThreadLocalRandom.current().nextInt(pairs.size() + 1), pair);
    }

    public Pair<T, U> remove() {
        ensureNotEmpty();
        return pairs.remove(ThreadLocalRandom.current().nextInt(pairs.size()));
    }

    public Pair<T, U> get() {
        ensureNotEmpty();
        return pairs.get(ThreadLocalRandom.current().nextInt(pairs.size()));
    }

    public int size() {
        return pairs.size();
    }

    public boolean isEmpty() {
        return pairs.isEmpty();
    }

    private void ensureNotEmpty() {
        if (isEmpty()) {
            throw new IllegalStateException("Pair bag is empty");
        }
    }
}

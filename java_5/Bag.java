import java.util.concurrent.ThreadLocalRandom;

public class Bag {
    private final Object[] elements;
    private int size;

    public Bag(int capacity) {
        if (capacity < 0) {
            throw new IllegalArgumentException("capacity must not be negative");
        }
        elements = new Object[capacity];
    }

    public final void add(Object element) {
        if (size == elements.length) {
            throw new IllegalStateException("Bag is full");
        }
        int index = ThreadLocalRandom.current().nextInt(size + 1);
        elements[size] = elements[index];
        elements[index] = element;
        size++;
    }

    public final Object remove() {
        ensureNotEmpty();
        int index = ThreadLocalRandom.current().nextInt(size);
        Object removed = elements[index];
        elements[index] = elements[size - 1];
        elements[size - 1] = null;
        size--;
        return removed;
    }

    public final Object get() {
        ensureNotEmpty();
        return elements[ThreadLocalRandom.current().nextInt(size)];
    }

    public final int size() {
        return size;
    }

    public final int capacity() {
        return elements.length;
    }

    public final boolean isEmpty() {
        return size == 0;
    }

    public final void clear() {
        for (int i = 0; i < size; i++) {
            elements[i] = null;
        }
        size = 0;
    }

    private void ensureNotEmpty() {
        if (isEmpty()) {
            throw new IllegalStateException("Bag is empty");
        }
    }
}

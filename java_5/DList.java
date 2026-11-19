import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Objects;

public final class DList<T, U> {
    private final List<T> values = new ArrayList<>();
    private final List<List<List<U>>> associations = new ArrayList<>();

    public void add(T value, Collection<? extends List<? extends U>> associatedLists) {
        if (associatedLists == null) {
            throw new IllegalArgumentException("associatedLists must not be null");
        }
        List<List<U>> copiedLists = new ArrayList<>();
        for (List<? extends U> list : associatedLists) {
            copiedLists.add(new ArrayList<>(list));
        }
        values.add(value);
        associations.add(copiedLists);
    }

    public T getValue(int index) {
        return values.get(index);
    }

    public List<List<U>> getAssociated(int index) {
        return immutableCopy(associations.get(index));
    }

    public List<List<U>> getAssociatedByValue(T value) {
        int index = indexOf(value);
        if (index < 0) {
            throw new IllegalArgumentException("No entry for value: " + value);
        }
        return getAssociated(index);
    }

    public Pair<T, List<List<U>>> removeAt(int index) {
        T value = values.remove(index);
        List<List<U>> associated = associations.remove(index);
        return new Pair<>(value, immutableCopy(associated));
    }

    public Pair<T, List<List<U>>> removeByValue(T value) {
        int index = indexOf(value);
        if (index < 0) {
            throw new IllegalArgumentException("No entry for value: " + value);
        }
        return removeAt(index);
    }

    public int indexOf(T value) {
        for (int i = 0; i < values.size(); i++) {
            if (Objects.equals(values.get(i), value)) {
                return i;
            }
        }
        return -1;
    }

    public int size() {
        return values.size();
    }

    public boolean isEmpty() {
        return values.isEmpty();
    }

    public void clear() {
        values.clear();
        associations.clear();
    }

    private List<List<U>> immutableCopy(List<List<U>> lists) {
        List<List<U>> copy = new ArrayList<>();
        for (List<U> list : lists) {
            copy.add(Collections.unmodifiableList(new ArrayList<>(list)));
        }
        return Collections.unmodifiableList(copy);
    }
}

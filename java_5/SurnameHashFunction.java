import java.util.Locale;

public final class SurnameHashFunction extends HashFunction<String> {
    public SurnameHashFunction(int tableSize) {
        super(tableSize);
    }

    @Override
    public int hash(String surname) {
        if (surname == null) {
            throw new IllegalArgumentException("surname must not be null");
        }
        int hash = 0;
        String normalized = surname.toUpperCase(Locale.ROOT);
        for (int i = 0; i < normalized.length(); i++) {
            hash = (hash + normalized.charAt(i)) % getTableSize();
        }
        return hash;
    }
}

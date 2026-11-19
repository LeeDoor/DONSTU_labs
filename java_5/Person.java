import java.util.Objects;
import java.util.Locale;

public final class Person implements Hashable<String> {
    private final String surname;
    private final String name;
    private final int age;

    public Person(String surname, String name, int age) {
        if (surname == null || surname.trim().isEmpty()) {
            throw new IllegalArgumentException("surname must not be blank");
        }
        if (name == null || name.trim().isEmpty()) {
            throw new IllegalArgumentException("name must not be blank");
        }
        if (age < 0) {
            throw new IllegalArgumentException("age must not be negative");
        }
        this.surname = surname.trim();
        this.name = name.trim();
        this.age = age;
    }

    public String getSurname() {
        return surname;
    }

    public String getName() {
        return name;
    }

    public int getAge() {
        return age;
    }

    @Override
    public boolean contains(String value) {
        return value != null
                && surname.toUpperCase(Locale.ROOT).equals(value.toUpperCase(Locale.ROOT));
    }

    @Override
    public String toString() {
        return surname + " " + name + ", " + age;
    }

    @Override
    public boolean equals(Object other) {
        if (this == other) {
            return true;
        }
        if (!(other instanceof Person)) {
            return false;
        }
        Person person = (Person) other;
        return age == person.age
                && surname.equals(person.surname)
                && name.equals(person.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(surname, name, age);
    }
}

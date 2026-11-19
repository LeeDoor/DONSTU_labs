import java.util.Arrays;
import java.util.List;

public final class Lab5Demo {
    private Lab5Demo() {
    }

    public static void main(String[] args) {
        checkPairAndBags();
        checkDListAndCoinChange();
        checkHashTable();
        System.out.println("Все проверки лабораторной работы пройдены.");
    }

    private static void checkPairAndBags() {
        Pair<Integer, String> pair = Pair.make_pair(7, "семь");
        require(pair.first == 7 && "семь".equals(pair.second), "Pair / make_pair");

        Bag bag = new Bag(2);
        bag.add(42);
        bag.add("текст");
        require(bag.size() == 2, "Bag size");
        Object removed = bag.remove();
        require(Integer.valueOf(42).equals(removed) || "текст".equals(removed), "Bag remove");

        PairBag pairBag = new PairBag(2);
        pairBag.add(Pair.make_pair(1, 2));
        pairBag.add(Pair.make_pair(1, "два"));
        require(pairBag.size() == 2, "PairBag accepts different pair types");

        GPairBag<Integer, String> genericBag = new GPairBag<>(2);
        genericBag.add(Pair.make_pair(1, "один"));
        require("один".equals(genericBag.remove().second), "GPairBag");

        GenericPairBag<Integer, String> standardBag = new GenericPairBag<>();
        standardBag.add(Pair.make_pair(2, "два"));
        require(standardBag.remove().first == 2, "GenericPairBag");
    }

    private static void checkDListAndCoinChange() {
        DList<Integer, Integer> list = new DList<>();
        list.add(4, Arrays.asList(Arrays.asList(1, 3), Arrays.asList(2, 2)));
        require(list.getAssociatedByValue(4).size() == 2, "DList associated lists");

        DList<Integer, Integer> result = CoinChange.solve(5, new int[]{1, 2, 3});
        List<List<Integer>> combinations = result.getAssociatedByValue(5);
        require(combinations.size() == 1 && combinations.get(0).size() == 2,
                "minimum-coin change for 5 with 1, 2, 3");
        require(CoinChange.minimumCoinCount(result, 5) == 2, "minimum coin count");
        require(combinations.get(0).contains(2) && combinations.get(0).contains(3),
                "coin combination");

        DList<Integer, Integer> alternatives = CoinChange.solve(8, new int[]{1, 4, 7, 9});
        require(alternatives.getAssociatedByValue(8).size() == 2,
                "all optimal coin combinations");
        require(CoinChange.minimumCoinCount(alternatives, 8) == 2,
                "minimum count with alternative combinations");
        require(CoinChange.minimumCoinCount(CoinChange.solve(3, new int[]{2}), 3) == -1,
                "unreachable amount");
    }

    private static void checkHashTable() {
        int capacity = 17;
        HashTable<Person, String> people = new HashTable<>(
                capacity, new SurnameHashFunction(capacity));
        people.add("Иванов", new Person("Иванов", "Иван", 20));
        people.add("Иванов", new Person("Иванов", "Петр", 25));
        people.add("Петров", new Person("Петров", "Анна", 30));
        people.add("AB", new Person("AB", "Алекс", 24));
        people.add("BA", new Person("BA", "Борис", 26));

        require(people.find("иванов").size() == 2, "case-insensitive hash lookup");
        require(people.find("AB").size() == 1 && people.find("BA").size() == 1,
                "distinct keys that collide in one bucket");
        require(people.remove("Иванов") == 2 && people.size() == 3, "hash-table removal");
    }

    private static void require(boolean condition, String description) {
        if (!condition) {
            throw new AssertionError("Проверка не пройдена: " + description);
        }
    }
}

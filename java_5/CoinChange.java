import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.TreeSet;

public final class CoinChange {
    private CoinChange() {
    }

    public static DList<Integer, Integer> solve(int amount, int[] denominations) {
        if (amount < 0) {
            throw new IllegalArgumentException("amount must not be negative");
        }
        if (denominations == null) {
            throw new IllegalArgumentException("denominations must not be null");
        }

        TreeSet<Integer> uniqueCoins = new TreeSet<>();
        for (int coin : denominations) {
            if (coin <= 0) {
                throw new IllegalArgumentException("coin denominations must be positive");
            }
            uniqueCoins.add(coin);
        }

        int unreachable = Integer.MAX_VALUE;
        int[] minimum = new int[amount + 1];
        Arrays.fill(minimum, unreachable);
        minimum[0] = 0;

        List<List<Integer>>[] combinations = newListArray(amount + 1);
        combinations[0].add(new ArrayList<>());

        for (int coin : uniqueCoins) {
            for (int value = coin; value <= amount; value++) {
                if (minimum[value - coin] == unreachable) {
                    continue;
                }
                int candidate = minimum[value - coin] + 1;
                if (candidate < minimum[value]) {
                    minimum[value] = candidate;
                    combinations[value].clear();
                }
                if (candidate == minimum[value]) {
                    for (List<Integer> previous : combinations[value - coin]) {
                        List<Integer> combination = new ArrayList<>(previous);
                        combination.add(coin);
                        combinations[value].add(combination);
                    }
                }
            }
        }

        DList<Integer, Integer> results = new DList<>();
        for (int value = 0; value <= amount; value++) {
            results.add(value, combinations[value]);
        }
        return results;
    }

    public static int minimumCoinCount(DList<Integer, Integer> results, int amount) {
        if (results == null) {
            throw new IllegalArgumentException("results must not be null");
        }
        List<List<Integer>> combinations = results.getAssociatedByValue(amount);
        return combinations.isEmpty() ? -1 : combinations.get(0).size();
    }

    @SuppressWarnings("unchecked")
    private static List<List<Integer>>[] newListArray(int length) {
        List<List<Integer>>[] lists = (List<List<Integer>>[]) new List<?>[length];
        for (int i = 0; i < length; i++) {
            lists[i] = new ArrayList<>();
        }
        return lists;
    }
}

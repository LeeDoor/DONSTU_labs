import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public final class Tournament {
    private static final int DEFAULT_TEAM_COUNT = 8;

    private Tournament() {
    }

    public static void main(String[] args) throws IOException {
        BufferedReader input = new BufferedReader(new InputStreamReader(System.in));
        System.out.print("Количество команд (степень двойки, не меньше 2): ");
        int teamCount = readTeamCount(input.readLine());
        if (!isValidTeamCount(teamCount)) {
            System.out.println("Некорректное число команд; используется значение "
                    + DEFAULT_TEAM_COUNT + ".");
            teamCount = DEFAULT_TEAM_COUNT;
        }
        System.out.println("Участвуют " + teamCount + " команд.");

        Bag teams = new Bag(teamCount);
        for (int i = 1; i <= teamCount; i++) {
            teams.add("Команда" + i);
        }

        int round = 1;
        while (teams.size() > 1) {
            GenericPairBag<String, String> matches = new GenericPairBag<>();
            while (!teams.isEmpty()) {
                String first = (String) teams.remove();
                String second = (String) teams.remove();
                matches.add(new Pair<>(first, second));
            }

            System.out.println("\nРаунд " + round + ":");
            while (!matches.isEmpty()) {
                Pair<String, String> match = matches.remove();
                int winner = readWinner(input, match);
                teams.add(winner == 1 ? match.first : match.second);
            }
            round++;
        }

        System.out.println("\nПобедитель турнира: " + teams.remove());
    }

    private static int readTeamCount(String line) {
        try {
            return Integer.parseInt(line == null ? "" : line.trim());
        } catch (NumberFormatException exception) {
            return -1;
        }
    }

    private static boolean isValidTeamCount(int count) {
        return count >= 2 && (count & (count - 1)) == 0;
    }

    private static int readWinner(BufferedReader input, Pair<String, String> match)
            throws IOException {
        while (true) {
            System.out.println("Матч: 1) " + match.first + "  2) " + match.second);
            System.out.print("Введите номер победителя (1 или 2): ");
            String answer = input.readLine();
            answer = answer == null ? "" : answer.trim();
            if ("1".equals(answer)) {
                return 1;
            }
            if ("2".equals(answer)) {
                return 2;
            }
            System.out.println("Введите 1 или 2.");
        }
    }
}

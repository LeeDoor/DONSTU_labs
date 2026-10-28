import java.util.Arrays;
import java.util.Locale;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Lab3 {
    private static final double EPS = 1e-9;

    private static final String FEDERAL_REGEX =
            "(\\+7|8)[ -]?(\\(\\d{3}\\)|\\d{3})[ -]?\\d{3}[ -]?\\d{2}[ -]?\\d{2}";
    private static final String MUNICIPAL_REGEX =
            "[23][ -]?\\d{2}[ -]?\\d{2}[ -]?\\d{2}";

    private static final Pattern FEDERAL_PATTERN = Pattern.compile(FEDERAL_REGEX);
    private static final Pattern MUNICIPAL_PATTERN = Pattern.compile(MUNICIPAL_REGEX);
    private static final Pattern PHONE_PATTERN =
            Pattern.compile("(?<!\\d)(" + FEDERAL_REGEX + "|" + MUNICIPAL_REGEX + ")(?!\\d)");

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        try {
            if (args.length > 0) {
                runTask(Integer.parseInt(args[0]), scanner);
                return;
            }

            while (true) {
                printMenu();
                System.out.print("Выберите задание (0 - выход): ");
                if (!scanner.hasNextLine()) {
                    break;
                }
                String line = scanner.nextLine().trim();
                if (line.isEmpty()) {
                    continue;
                }
                int task;
                try {
                    task = Integer.parseInt(line);
                } catch (NumberFormatException e) {
                    System.out.println("Введите число от 0 до 10.");
                    continue;
                }
                if (task == 0) {
                    break;
                }
                runTask(task, scanner);
                System.out.println();
            }
        } finally {
            scanner.close();
        }
    }

    private static void printMenu() {
        System.out.println("Лабораторная работа №3. Управляющие конструкции Java");
        System.out.println(" 1 - табулирование функций");
        System.out.println(" 2 - наибольший отрицательный элемент двумерного массива");
        System.out.println(" 3 - сортировка строк матрицы 3x3");
        System.out.println(" 4 - взаимное расположение окружностей (int)");
        System.out.println(" 5 - взаимное расположение окружностей (enum)");
        System.out.println(" 6 - интегрирование методом левых прямоугольников");
        System.out.println(" 7 - перевод целого числа в систему счисления 2..8");
        System.out.println(" 8 - схема Горнера");
        System.out.println(" 9 - регулярные выражения для телефонных номеров");
        System.out.println("10 - извлечение телефонных номеров из строки");
    }

    private static void runTask(int task, Scanner scanner) {
        switch (task) {
            case 1 -> task1();
            case 2 -> task2();
            case 3 -> task3();
            case 4 -> task4(scanner);
            case 5 -> task5(scanner);
            case 6 -> task6(scanner);
            case 7 -> task7(scanner);
            case 8 -> task8(scanner);
            case 9 -> task9();
            case 10 -> task10();
            default -> System.out.println("Неизвестное задание: " + task);
        }
    }

    private static void task1() {
        System.out.println("Задание 1. Табулирование функций");
        System.out.printf(Locale.US, "%10s %15s %15s%n", "x", "sin(x)", "e^x/x*lg(x)");
        double step = Math.PI / 15.0;
        for (int i = 1; i <= 15; i++) {
            double x = i * step;
            double y = Math.exp(x) / x * Math.log10(x);
            System.out.printf(Locale.US, "%10.5f %15.7e %15.7e%n", x, Math.sin(x), y);
        }
    }

    private static void task2() {
        System.out.println("Задание 2. Наибольший отрицательный элемент");
        int[][] matrix = {
                {-5, 3, -17, 8},
                {-2},
                {4, -9, -1, 6, -12, 0},
                {-30, -4}
        };

        printMatrix(matrix);
        Integer result = maxNegative(matrix);
        if (result == null) {
            System.out.println("Отрицательных элементов нет.");
        } else {
            System.out.println("Наибольший отрицательный элемент: " + result);
        }
    }

    private static Integer maxNegative(int[][] matrix) {
        Integer best = null;
        for (int[] row : matrix) {
            for (int value : row) {
                if (value < 0 && (best == null || value > best)) {
                    best = value;
                }
            }
        }
        return best;
    }

    private static void task3() {
        System.out.println("Задание 3. Сортировка строк матрицы 3x3");
        int[][] matrix = {
                {3, 1, 2},
                {9, 7, 8},
                {6, 5, 4}
        };

        System.out.println("До сортировки:");
        printMatrix(matrix);

        for (int[] row : matrix) {
            Arrays.sort(row);
        }

        System.out.println("После сортировки:");
        printMatrix(matrix);
    }

    private static void printMatrix(int[][] matrix) {
        for (int[] row : matrix) {
            for (int value : row) {
                System.out.printf("%5d", value);
            }
            System.out.println();
        }
    }

    private static void task4(Scanner scanner) {
        System.out.println("Задание 4. Взаимное расположение окружностей (int)");
        double[] values = readCircleValues(scanner);
        if (values == null) {
            return;
        }

        int code = CircleRelation.relationCode(
                values[0], values[1], values[2], values[3], values[4], values[5]);
        System.out.println("Код результата: " + code);
        System.out.println("Результат: " + CircleRelation.description(code));
    }

    private static void task5(Scanner scanner) {
        System.out.println("Задание 5. Взаимное расположение окружностей (enum)");
        double[] values = readCircleValues(scanner);
        if (values == null) {
            return;
        }

        CircleRelation.Position position = CircleRelation.relation(
                values[0], values[1], values[2], values[3], values[4], values[5]);
        System.out.println("Значение перечисления: " + position);
        System.out.println("Код: " + position.getCode());
        System.out.println("Результат: " + position.getDescription());
    }

    private static double[] readCircleValues(Scanner scanner) {
        double[] values = readDoubles(scanner,
                "Введите x1 y1 r1 x2 y2 r2 (Enter - пример 0 0 5 4 0 3): ",
                new double[]{0, 0, 5, 4, 0, 3});
        if (values[2] <= 0 || values[5] <= 0) {
            System.out.println("Радиусы должны быть положительными.");
            return null;
        }
        return values;
    }

    private static void task6(Scanner scanner) {
        System.out.println("Задание 6. Интегрирование методом левых прямоугольников");
        double[] interval = readDoubles(scanner, "Введите a и b (Enter - 0 4): ", new double[]{0, 4});
        double a = interval[0];
        double b = interval[1];
        if (b <= a) {
            System.out.println("Значение b должно быть больше a.");
            return;
        }

        int points = 101;
        double[] x = new double[points];
        double[] y = new double[points];
        double step = (b - a) / (points - 1);
        for (int i = 0; i < points; i++) {
            x[i] = a + i * step;
            y[i] = function6(x[i]);
        }

        System.out.printf(Locale.US, "%3s %12s %16s%n", "i", "x", "y");
        for (int i = 0; i < points; i++) {
            System.out.printf(Locale.US, "%3d %12.6f %16.8f%n", i, x[i], y[i]);
        }

        double numeric = leftRectangles(x, y);
        double exact = primitive6(b) - primitive6(a);
        double error = Math.abs(numeric - exact);

        System.out.printf(Locale.US, "Численное значение: %.10f%n", numeric);
        System.out.printf(Locale.US, "Точное значение:     %.10f%n", exact);
        System.out.printf(Locale.US, "Абсолютная погрешность: %.10e%n", error);
    }

    private static double function6(double x) {
        return Math.exp(x) - Math.pow(x, 3);
    }

    private static double primitive6(double x) {
        return Math.exp(x) - Math.pow(x, 4) / 4.0;
    }

    private static double leftRectangles(double[] x, double[] y) {
        double sum = 0;
        for (int i = 0; i < x.length - 1; i++) {
            sum += y[i] * (x[i + 1] - x[i]);
        }
        return sum;
    }

    private static void task7(Scanner scanner) {
        System.out.println("Задание 7. Перевод числа в систему счисления 2..8");
        int value = readInt(scanner, "Введите целое число (Enter - 12345): ", 12345);
        int base = readInt(scanner, "Введите основание системы счисления (Enter - 3): ", 3);
        while (base < 2 || base > 8) {
            System.out.println("Основание должно быть от 2 до 8.");
            base = readInt(scanner, "Введите основание системы счисления (Enter - 3): ", 3);
        }

        String own = toBase(value, base);
        String check = Integer.toString(value, base);

        System.out.println("Число: " + value);
        System.out.println("Основание: " + base);
        System.out.println("Собственный результат: " + own);
        System.out.println("Проверка Integer.toString: " + check);
        System.out.println("Результаты совпадают: " + own.equals(check));
    }

    private static String toBase(int value, int base) {
        if (value == 0) {
            return "0";
        }
        StringBuilder result = new StringBuilder();
        long number = Math.abs((long) value);
        while (number > 0) {
            result.append(Character.forDigit((int) (number % base), base));
            number /= base;
        }
        if (value < 0) {
            result.append('-');
        }
        return result.reverse().toString();
    }

    private static void task8(Scanner scanner) {
        System.out.println("Задание 8. Схема Горнера");
        double[] coefficients = {2, -3, 0, 5, -1};
        double x = readDouble(scanner, "Введите x (Enter - 2): ", 2.0);

        System.out.println("Коэффициенты от старшей степени: " + Arrays.toString(coefficients));
        System.out.printf(Locale.US, "P(%.5f) = %.10f%n", x, horner(coefficients, x));
    }

    private static double horner(double[] coefficients, double x) {
        double result = coefficients[0];
        for (int i = 1; i < coefficients.length; i++) {
            result = result * x + coefficients[i];
        }
        return result;
    }

    private static void task9() {
        System.out.println("Задание 9. Регулярные выражения для телефонных номеров");
        System.out.println("Федеральный номер: " + FEDERAL_REGEX);
        System.out.println("Муниципальный номер: " + MUNICIPAL_REGEX);
        System.out.println();

        System.out.println("Федеральные номера:");
        String[] federalValid = {
                "+79043781661",
                "+7 904 378 1661",
                "+7 904 378 16 61",
                "+7-904-378-16-61",
                "+7(904)3781661",
                "+7(904) 378-16 61",
                "89043781661",
                "8 904 378-16-61"
        };
        String[] federalInvalid = {
                "79043781661",
                "+7904378166",
                "+790437816611",
                "8(90)43781661",
                "+7 904 37 81 66 1",
                "8-904-378-16",
                "+7 904 378 16-6"
        };
        for (String number : federalValid) {
            reportFederal(number, true);
        }
        for (String number : federalInvalid) {
            reportFederal(number, false);
        }

        System.out.println();
        System.out.println("Муниципальные номера:");
        String[] municipalValid = {
                "220-30-40",
                "2 20 30 40",
                "320 30 40",
                "2-20-30-40",
                "3 20-30-40"
        };
        String[] municipalInvalid = {
                "120-30-40",
                "220-3-040",
                "220-30-4",
                "22030400"
        };
        for (String number : municipalValid) {
            reportMunicipal(number, true);
        }
        for (String number : municipalInvalid) {
            reportMunicipal(number, false);
        }

        String text = "Мои номера 220-30-40 и 8904-378-16-61 не считая служебных";
        System.out.println();
        reportMunicipal(text, true);
    }

    private static void reportFederal(String number, boolean expected) {
        boolean actual = FEDERAL_PATTERN.matcher(number).matches();
        System.out.printf("  %-24s %-5b ожидалось %-5b %s%n",
                number, actual, expected, actual == expected ? "OK" : "FAIL");
    }

    private static void reportMunicipal(String text, boolean expected) {
        boolean actual = containsMunicipal(text);
        System.out.printf("  %-70s %-5b ожидалось %-5b %s%n",
                text, actual, expected, actual == expected ? "OK" : "FAIL");
    }

    private static boolean containsMunicipal(String text) {
        Matcher matcher = PHONE_PATTERN.matcher(text);
        while (matcher.find()) {
            if (MUNICIPAL_PATTERN.matcher(matcher.group()).matches()) {
                return true;
            }
        }
        return false;
    }

    private static void task10() {
        System.out.println("Задание 10. Извлечение телефонных номеров из строки");
        String text = "Мои номера 220-30-40 и 8904-378-16-61 не считая служебных";
        System.out.println("Строка: " + text);

        Matcher matcher = PHONE_PATTERN.matcher(text);
        int count = 0;
        while (matcher.find()) {
            count++;
            String number = matcher.group();
            String type = FEDERAL_PATTERN.matcher(number).matches()
                    ? "федеральный"
                    : "муниципальный";
            System.out.printf("%d. %-20s %s%n", count, number, type);
        }
        System.out.println("Всего найдено номеров: " + count);
    }

    private static String readLine(Scanner scanner, String prompt) {
        System.out.print(prompt);
        if (!scanner.hasNextLine()) {
            return "";
        }
        return scanner.nextLine().trim();
    }

    private static double[] readDoubles(Scanner scanner, String prompt, double[] defaults) {
        while (true) {
            String line = readLine(scanner, prompt);
            if (line.isEmpty()) {
                return defaults.clone();
            }
            String[] parts = line.split("[\\s,]+");
            if (parts.length != defaults.length) {
                System.out.println("Нужно ввести " + defaults.length + " чисел.");
                continue;
            }
            double[] values = new double[defaults.length];
            try {
                for (int i = 0; i < parts.length; i++) {
                    values[i] = Double.parseDouble(parts[i].replace(',', '.'));
                }
                return values;
            } catch (NumberFormatException e) {
                System.out.println("Введите числа, например: " + Arrays.toString(defaults));
            }
        }
    }

    private static double readDouble(Scanner scanner, String prompt, double defaultValue) {
        while (true) {
            String line = readLine(scanner, prompt);
            if (line.isEmpty()) {
                return defaultValue;
            }
            try {
                return Double.parseDouble(line.replace(',', '.'));
            } catch (NumberFormatException e) {
                System.out.println("Введите действительное число.");
            }
        }
    }

    private static int readInt(Scanner scanner, String prompt, int defaultValue) {
        while (true) {
            String line = readLine(scanner, prompt);
            if (line.isEmpty()) {
                return defaultValue;
            }
            try {
                return Integer.parseInt(line);
            } catch (NumberFormatException e) {
                System.out.println("Введите целое число.");
            }
        }
    }

    private static final class CircleRelation {
        enum Position {
            INTERSECT(1, "пересекаются в двух точках"),
            TOUCH(2, "касаются (одна общая точка)"),
            COINCIDE(3, "совпадают"),
            DISJOINT(4, "не пересекаются, ни одна не вложена в другую"),
            SECOND_INSIDE_FIRST(5, "вторая окружность вложена в первую"),
            FIRST_INSIDE_SECOND(6, "первая окружность вложена во вторую");

            private final int code;
            private final String description;

            Position(int code, String description) {
                this.code = code;
                this.description = description;
            }

            int getCode() {
                return code;
            }

            String getDescription() {
                return description;
            }
        }

        private CircleRelation() {
        }

        static Position relation(double x1, double y1, double r1,
                                 double x2, double y2, double r2) {
            double distance = Math.hypot(x1 - x2, y1 - y2);
            double sum = r1 + r2;
            double difference = Math.abs(r1 - r2);

            if (distance <= EPS && difference <= EPS) {
                return Position.COINCIDE;
            }
            if (Math.abs(distance - sum) <= EPS) {
                return Position.TOUCH;
            }
            if (distance > sum + EPS) {
                return Position.DISJOINT;
            }
            if (Math.abs(distance - difference) <= EPS) {
                return Position.TOUCH;
            }
            if (distance > difference + EPS) {
                return Position.INTERSECT;
            }
            return r1 > r2 ? Position.SECOND_INSIDE_FIRST : Position.FIRST_INSIDE_SECOND;
        }

        static int relationCode(double x1, double y1, double r1,
                                double x2, double y2, double r2) {
            return relation(x1, y1, r1, x2, y2, r2).getCode();
        }

        static String description(int code) {
            for (Position position : Position.values()) {
                if (position.getCode() == code) {
                    return position.getDescription();
                }
            }
            return "неизвестный код";
        }
    }
}

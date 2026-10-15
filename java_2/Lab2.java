public class Lab2 {
    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("Usage: java Lab2 <task> [arguments]");
            return;
        }

        int task = Integer.parseInt(args[0]);

        switch (task) {
            case 1 -> task1();
            case 2 -> task2();
            case 3 -> task3();
            case 4 -> task4(args);
            case 5 -> task5(args);
            case 6 -> task6();
            case 7 -> task7(args);
            case 8 -> task8();
            case 9 -> task9(args);
            case 10 -> task10(args);
            default -> System.out.println("Unknown task: " + task);
        }
    }

    static void task1() {
        System.out.println("Type\t\tMIN_VALUE\t\tMAX_VALUE");
        System.out.println("byte\t\t" + Byte.MIN_VALUE + "\t\t\t" + Byte.MAX_VALUE);
        System.out.println("short\t\t" + Short.MIN_VALUE + "\t\t\t" + Short.MAX_VALUE);
        System.out.println("int\t\t" + Integer.MIN_VALUE + "\t\t" + Integer.MAX_VALUE);
        System.out.println("long\t\t" + Long.MIN_VALUE + "\t\t" + Long.MAX_VALUE);
        System.out.println("float\t\t" + Float.MIN_VALUE + "\t\t" + Float.MAX_VALUE);
        System.out.println("double\t\t" + Double.MIN_VALUE + "\t\t" + Double.MAX_VALUE);
    }

    static void task2() {
        int[] a = {
            -2, 3, -8, 4, -1, 5, 6, -4, 7, 2,
            -3, 9, 1, -5, 8, 2, -6, 4, 3, -10
        };

        double product = 1.0;
        int count = 0;

        for (int x : a) {
            if (x < 0) {
                product *= Math.abs((double) x);
                count++;
            }
        }

        if (count == 0) {
            System.out.println("There are no negative elements.");
            return;
        }

        double mean = Math.pow(product, 1.0 / count);
        System.out.println("Negative elements: " + count);
        System.out.println("Geometric mean of absolute values: " + (-mean));
    }

    static void radar(double R, double r, double x, double y) {
        double distance = Math.sqrt(x * x + y * y);

        System.out.println("Distance: " + distance);

        if (distance > R) {
            System.out.println("Не обнаружен");
        } else if (distance > r) {
            System.out.println("Обнаружен");
        } else {
            System.out.println("Тревога");
        }
    }

    static void task3() {
        double R = 10.0;
        double r = 3.0;
        double x = 4.0;
        double y = 2.0;

        radar(R, r, x, y);
    }

    static void task4(String[] args) {
        if (args.length != 5) {
            System.out.println("Usage: java Lab2 4 <R> <r> <x> <y>");
            return;
        }

        double R = Double.parseDouble(args[1]);
        double r = Double.parseDouble(args[2]);
        double x = Double.parseDouble(args[3]);
        double y = Double.parseDouble(args[4]);

        if (R < 0 || r < 0 || r >= R) {
            System.out.println("Require R > r >= 0.");
            return;
        }

        radar(R, r, x, y);
    }

    static void task5(String[] args) {
        if (args.length != 2) {
            System.out.println("Usage: java Lab2 5 <integer>");
            return;
        }

        int value = Integer.parseInt(args[1]);

        System.out.println("Decimal: " + value);
        System.out.println("Binary: " + Integer.toString(value, 2));
        System.out.println("Octal: " + Integer.toString(value, 8));
        System.out.println("Hexadecimal: " + Integer.toString(value, 16));
    }

    static void printUnicodeTable(int start, int rows, int columns) {
        System.out.print("     ");

        for (int col = 0; col < columns; col++) {
            System.out.printf("%x ", col);
        }

        System.out.println();

        for (int row = 0; row < rows; row++) {
            int code = start + row * columns;
            System.out.printf("%04x ", code);

            for (int col = 0; col < columns; col++) {
                int current = code + col;
                System.out.print(Character.toString(current) + " ");
            }

            System.out.println();
        }
    }

    static void task6() {
        printUnicodeTable(0x0400, 16, 16);
        System.out.println();
        printUnicodeTable(0x20A0, 2, 16);
    }

    static void task7(String[] args) {
        if (args.length != 2) {
            System.out.println("Usage: java Lab2 7 <text>");
            return;
        }

        String text = args[1];
        int letters = 0;
        int lowercase = 0;
        int uppercase = 0;
        int digits = 0;
        int asciiDigits = 0;
        int nonAsciiDigits = 0;
        int other = 0;

        for (int i = 0; i < text.length(); i++) {
            int codePoint = text.codePointAt(i);

            if (Character.isLetter(codePoint)) {
                letters++;

                if (Character.isLowerCase(codePoint)) {
                    lowercase++;
                }

                if (Character.isUpperCase(codePoint)) {
                    uppercase++;
                }
            } else if (Character.isDigit(codePoint)) {
                digits++;

                if (codePoint >= '0' && codePoint <= '9') {
                    asciiDigits++;
                } else {
                    nonAsciiDigits++;
                }
            } else {
                other++;
            }

            if (Character.charCount(codePoint) == 2) {
                i++;
            }
        }

        System.out.println("Letters: " + letters);
        System.out.println("Lowercase letters: " + lowercase);
        System.out.println("Uppercase letters: " + uppercase);
        System.out.println("Digits: " + digits);
        System.out.println("ASCII digits (0-9): " + asciiDigits);
        System.out.println("Non-ASCII digits: " + nonAsciiDigits);
        System.out.println("Other characters: " + other);
        System.out.println("Total Unicode code points: "
                + (letters + digits + other));
    }

    static void task8() {
        String text = "Java programming";
        String part = "gram";

        System.out.println("Text: " + text);
        System.out.println("Substring: " + part);
        System.out.println("First occurrence: " + text.indexOf(part));
        System.out.println("Search from index 5: " + text.indexOf(part, 5));
        System.out.println("Missing substring: " + text.indexOf("Python"));
    }

    static void task9(String[] args) {
        if (args.length != 3) {
            System.out.println("Usage: java Lab2 9 <text> <substring>");
            return;
        }

        String text = args[1];
        String part = args[2];

        if (part.isEmpty()) {
            System.out.println("Substring must not be empty.");
            return;
        }

        int count = 0;
        int position = 0;

        while ((position = text.indexOf(part, position)) != -1) {
            count++;
            position += part.length();
        }

        System.out.println("Occurrences: " + count);
    }

    static void task10(String[] args) {
        if (args.length != 2) {
            System.out.println("Usage: java Lab2 10 <text>");
            return;
        }

        String text = args[1];

        for (int i = 0; i < text.length(); i++) {
            String rotation = text.substring(i) + text.substring(0, i);
            System.out.println(rotation);
        }
    }
}


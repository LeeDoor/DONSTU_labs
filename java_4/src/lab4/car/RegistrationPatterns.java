package lab4.car;

import java.util.regex.Pattern;

final class RegistrationPatterns {
    static final Pattern STANDARD = Pattern.compile(
            "^[АВЕКМНОРСТУХ] \\d{3} [АВЕКМНОРСТУХ]{2} \\d{2,3} RUS$");
    static final Pattern SPECIAL = Pattern.compile("^SP-\\d{4,6}$");

    private RegistrationPatterns() {
    }
}

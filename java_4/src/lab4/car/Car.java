package lab4.car;

import java.util.Objects;
import java.util.Optional;
import java.util.regex.Pattern;

public abstract class Car {
    private final String make;
    private final int wheelCount;
    private final Pattern registrationPattern;
    private String color;
    private Engine engine;
    private String registrationNumber;

    protected Car(String make, String color, int wheelCount, Engine engine,
                  Pattern registrationPattern, String registrationNumber) {
        if (make == null || make.isBlank()) {
            throw new IllegalArgumentException("Car make must not be blank.");
        }
        if (wheelCount <= 0) {
            throw new IllegalArgumentException("Wheel count must be positive.");
        }
        this.make = make.trim();
        this.color = requireText(color, "Color");
        this.wheelCount = wheelCount;
        this.engine = Objects.requireNonNull(engine, "Engine must not be null.");
        this.registrationPattern = Objects.requireNonNull(
                registrationPattern, "Registration pattern must not be null.");
        setRegistrationNumber(registrationNumber);
    }

    public final String getMake() {
        return make;
    }

    public final String getColor() {
        return color;
    }

    public final void setColor(String color) {
        this.color = requireText(color, "Color");
    }

    public final int getWheelCount() {
        return wheelCount;
    }

    public final Engine getEngine() {
        return engine;
    }

    public final void setEngine(Engine engine) {
        this.engine = Objects.requireNonNull(engine, "Engine must not be null.");
    }

    public final Optional<String> getRegistrationNumber() {
        return Optional.ofNullable(registrationNumber);
    }

    public final void setRegistrationNumber(String registrationNumber) {
        if (registrationNumber == null) {
            this.registrationNumber = null;
            return;
        }

        String normalized = registrationNumber.trim();
        if (!registrationPattern.matcher(normalized).matches()) {
            throw new IllegalArgumentException(
                    "Invalid registration number: " + registrationNumber);
        }
        this.registrationNumber = normalized;
    }

    public final boolean trySetRegistrationNumber(String registrationNumber) {
        try {
            setRegistrationNumber(registrationNumber);
            return true;
        } catch (IllegalArgumentException exception) {
            return false;
        }
    }

    public abstract VehicleType getType();

    public final String describe() {
        return String.format("%s %s, %s, %d wheels, %s, registration: %s",
                getType(), make, color, wheelCount, engine,
                getRegistrationNumber().orElse("not assigned"));
    }

    private static String requireText(String value, String fieldName) {
        if (value == null || value.isBlank()) {
            throw new IllegalArgumentException(fieldName + " must not be blank.");
        }
        return value.trim();
    }
}

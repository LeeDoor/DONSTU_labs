package lab4.car;

public class SpecialCar extends Car {
    private final String purpose;

    public SpecialCar(String make, String color, Engine engine, int wheelCount,
                      String purpose, String registrationNumber) {
        super(make, color, wheelCount, engine,
                RegistrationPatterns.SPECIAL, registrationNumber);
        if (purpose == null || purpose.isBlank()) {
            throw new IllegalArgumentException("Special vehicle purpose must not be blank.");
        }
        this.purpose = purpose.trim();
    }

    public String getPurpose() {
        return purpose;
    }

    @Override
    public final VehicleType getType() {
        return VehicleType.SPECIAL;
    }
}

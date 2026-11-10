package lab4.car;

public class Truck extends Car {
    private final double payloadTons;

    public Truck(String make, String color, Engine engine, int wheelCount,
                 double payloadTons, String registrationNumber) {
        super(make, color, wheelCount, engine,
                RegistrationPatterns.STANDARD, registrationNumber);
        if (!Double.isFinite(payloadTons) || payloadTons <= 0) {
            throw new IllegalArgumentException("Payload must be positive and finite.");
        }
        this.payloadTons = payloadTons;
    }

    public double getPayloadTons() {
        return payloadTons;
    }

    @Override
    public final VehicleType getType() {
        return VehicleType.TRUCK;
    }
}

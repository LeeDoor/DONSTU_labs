package lab4.car;

public class PassengerCar extends Car {
    private final int doorCount;

    public PassengerCar(String make, String color, Engine engine, int doorCount,
                        String registrationNumber) {
        super(make, color, 4, engine, RegistrationPatterns.STANDARD, registrationNumber);
        if (doorCount <= 0) {
            throw new IllegalArgumentException("Door count must be positive.");
        }
        this.doorCount = doorCount;
    }

    public int getDoorCount() {
        return doorCount;
    }

    @Override
    public final VehicleType getType() {
        return VehicleType.PASSENGER;
    }
}

package lab4.car;

public final class Bus extends Car {
    private final int passengerCapacity;

    public Bus(String make, String color, Engine engine, int wheelCount,
               int passengerCapacity, String registrationNumber) {
        super(make, color, wheelCount, engine,
                RegistrationPatterns.STANDARD, registrationNumber);
        if (passengerCapacity <= 0) {
            throw new IllegalArgumentException("Passenger capacity must be positive.");
        }
        this.passengerCapacity = passengerCapacity;
    }

    public int getPassengerCapacity() {
        return passengerCapacity;
    }

    @Override
    public VehicleType getType() {
        return VehicleType.BUS;
    }
}

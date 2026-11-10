package lab4.car;

import java.util.Objects;
import java.util.Locale;

public final class Engine {
    private final String serialNumber;
    private final int powerHorsepower;
    private final double displacementLiters;
    private final FuelType fuelType;
    private final int cylinderCount;

    public Engine(String serialNumber, int powerHorsepower, double displacementLiters,
                  FuelType fuelType, int cylinderCount) {
        if (serialNumber == null || serialNumber.isBlank()) {
            throw new IllegalArgumentException("Engine serial number must not be blank.");
        }
        if (powerHorsepower <= 0) {
            throw new IllegalArgumentException("Engine power must be positive.");
        }
        if (!Double.isFinite(displacementLiters) || displacementLiters <= 0) {
            throw new IllegalArgumentException("Engine displacement must be positive and finite.");
        }
        if (cylinderCount <= 0) {
            throw new IllegalArgumentException("Cylinder count must be positive.");
        }

        this.serialNumber = serialNumber.trim();
        this.powerHorsepower = powerHorsepower;
        this.displacementLiters = displacementLiters;
        this.fuelType = Objects.requireNonNull(fuelType, "Fuel type must not be null.");
        this.cylinderCount = cylinderCount;
    }

    public String getSerialNumber() {
        return serialNumber;
    }

    public int getPowerHorsepower() {
        return powerHorsepower;
    }

    public double getDisplacementLiters() {
        return displacementLiters;
    }

    public FuelType getFuelType() {
        return fuelType;
    }

    public int getCylinderCount() {
        return cylinderCount;
    }

    @Override
    public String toString() {
        return String.format(Locale.ROOT, "%s, %d hp, %.1f l, %s, %d cylinders",
                serialNumber, powerHorsepower, displacementLiters, fuelType, cylinderCount);
    }
}

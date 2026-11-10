package lab4.car;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Objects;
import java.util.Optional;

public final class FleetBase {
    private final Car[] vehicles;
    private final VehicleStatus[] statuses;
    private int size;

    public FleetBase(int capacity) {
        if (capacity <= 0) {
            throw new IllegalArgumentException("Fleet capacity must be positive.");
        }
        vehicles = new Car[capacity];
        statuses = new VehicleStatus[capacity];
    }

    public int getCapacity() {
        return vehicles.length;
    }

    public int getVehicleCount() {
        return size;
    }

    public void add(Car car) {
        Objects.requireNonNull(car, "Car must not be null.");
        if (findIndex(car) >= 0) {
            throw new IllegalArgumentException("This car is already registered at the base.");
        }
        if (size == vehicles.length) {
            throw new IllegalStateException("Fleet base is full.");
        }

        vehicles[size] = car;
        statuses[size] = VehicleStatus.ON_BASE;
        size++;
    }

    public boolean remove(Car car) {
        int index = findIndex(Objects.requireNonNull(car, "Car must not be null."));
        if (index < 0) {
            return false;
        }
        int last = size - 1;
        vehicles[index] = vehicles[last];
        statuses[index] = statuses[last];
        vehicles[last] = null;
        statuses[last] = null;
        size--;
        return true;
    }

    public void dispatchToTrip(Car car) {
        changeStatus(car, VehicleStatus.ON_BASE, VehicleStatus.IN_TRIP);
    }

    public void sendToRepair(Car car) {
        int index = requireIndex(car);
        if (statuses[index] == VehicleStatus.IN_REPAIR) {
            throw new IllegalStateException("Car is already in repair.");
        }
        statuses[index] = VehicleStatus.IN_REPAIR;
    }

    public void returnToBase(Car car) {
        int index = requireIndex(car);
        if (statuses[index] == VehicleStatus.ON_BASE) {
            throw new IllegalStateException("Car is already on the base.");
        }
        statuses[index] = VehicleStatus.ON_BASE;
    }

    public Optional<VehicleStatus> getStatus(Car car) {
        int index = findIndex(Objects.requireNonNull(car, "Car must not be null."));
        return index < 0 ? Optional.empty() : Optional.of(statuses[index]);
    }

    public List<Car> listReadyOnBase() {
        return listByStatus(VehicleStatus.ON_BASE);
    }

    public List<Car> listInTrip() {
        return listByStatus(VehicleStatus.IN_TRIP);
    }

    public List<Car> listInRepair() {
        return listByStatus(VehicleStatus.IN_REPAIR);
    }

    private void changeStatus(Car car, VehicleStatus expected, VehicleStatus next) {
        int index = requireIndex(car);
        if (statuses[index] != expected) {
            throw new IllegalStateException(
                    "Cannot change status from " + statuses[index] + " to " + next + ".");
        }
        statuses[index] = next;
    }

    private List<Car> listByStatus(VehicleStatus status) {
        List<Car> result = new ArrayList<>();
        for (int i = 0; i < size; i++) {
            if (statuses[i] == status) {
                result.add(vehicles[i]);
            }
        }
        return Collections.unmodifiableList(result);
    }

    private int requireIndex(Car car) {
        int index = findIndex(Objects.requireNonNull(car, "Car must not be null."));
        if (index < 0) {
            throw new IllegalArgumentException("Car is not registered at this base.");
        }
        return index;
    }

    private int findIndex(Car car) {
        for (int i = 0; i < size; i++) {
            if (vehicles[i] == car) {
                return i;
            }
        }
        return -1;
    }
}

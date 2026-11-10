package lab4;

import lab4.car.Bus;
import lab4.car.Engine;
import lab4.car.FleetBase;
import lab4.car.FuelType;
import lab4.car.PassengerCar;
import lab4.car.VehicleStatus;
import lab4.complex.Complex;
import lab4.complex.ComplexFunctions;
import lab4.graph.Axis;
import lab4.graph.Chart;
import lab4.graph.Curve;
import lab4.graph.Grid;
import lab4.graph.SamplePoint;

import java.util.List;

public final class Lab4Tests {
    private static final double EPSILON = 1e-10;

    private Lab4Tests() {
    }

    public static void main(String[] args) {
        testCarRegistrationAndImmutability();
        testComplexArithmeticAndFunctions();
        testFleetBaseTransitions();
        testChartRendering();
        System.out.println("All lab4 tests passed.");
    }

    private static void testCarRegistrationAndImmutability() {
        PassengerCar car = new PassengerCar("Lada", "white", engine(), 4, null);
        check(car.getRegistrationNumber().isEmpty(), "new car may have no plate");
        check(car.trySetRegistrationNumber("А 123 ВС 61 RUS"), "valid plate accepted");
        check(!car.trySetRegistrationNumber("A 123 BC 61 RUS"), "Latin plate letters rejected");
        check("А 123 ВС 61 RUS".equals(car.getRegistrationNumber().orElseThrow()),
                "invalid plate does not replace existing plate");
        expect(IllegalArgumentException.class,
                () -> car.setRegistrationNumber("invalid"), "invalid plate setter");

        Bus bus = new Bus("PAZ", "yellow", engine(), 4, 30, null);
        check(bus.getWheelCount() == 4, "bus wheel count fixed by its type");
    }

    private static void testComplexArithmeticAndFunctions() {
        Complex value = new Complex(3, 2);
        Complex quotient = value.divide(new Complex(1, -4));
        checkClose(quotient.getReal(), -5.0 / 17.0, "complex quotient real part");
        checkClose(quotient.getImaginary(), 14.0 / 17.0, "complex quotient imaginary part");
        check(value.add(2).equals(new Complex(5, 2)), "real scalar addition");
        check(value.conjugate().equals(new Complex(3, -2)), "complex conjugate");
        expect(ArithmeticException.class, () -> value.divide(new Complex(0, 0)),
                "division by zero");

        Complex real = new Complex(0.7, 0);
        checkClose(ComplexFunctions.sin(real).getReal(), Math.sin(0.7), "sin real argument");
        checkClose(ComplexFunctions.exp(real).getReal(), Math.exp(0.7), "exp real argument");
        checkClose(ComplexFunctions.cos(real).getReal(), Math.cos(0.7), "cos real argument");
        checkClose(ComplexFunctions.tan(real).getReal(), Math.tan(0.7), "tan real argument");
        checkClose(ComplexFunctions.atan(new Complex(0.4, 0)).getReal(),
                Math.atan(0.4), "atan real argument");
        check(new Complex(-0.0, 0).equals(new Complex(0, 0)),
                "positive and negative zero compare equal");
        check(new Complex(-0.0, 0).hashCode() == new Complex(0, 0).hashCode(),
                "equal complex numbers have matching hashes");
    }

    private static void testFleetBaseTransitions() {
        FleetBase base = new FleetBase(1);
        PassengerCar car = new PassengerCar("Lada", "white", engine(), 4, null);
        base.add(car);
        check(base.getStatus(car).orElseThrow() == VehicleStatus.ON_BASE, "initial base status");
        expect(IllegalArgumentException.class, () -> base.add(car), "duplicate vehicle");
        expect(IllegalStateException.class,
                () -> base.add(new PassengerCar("GAZ", "blue", engine(), 4, null)),
                "capacity limit");
        base.dispatchToTrip(car);
        check(base.listInTrip().size() == 1, "dispatched vehicle listed in trip");
        expect(IllegalStateException.class, () -> base.dispatchToTrip(car),
                "invalid repeated dispatch");
        base.returnToBase(car);
        base.sendToRepair(car);
        check(base.listInRepair().size() == 1, "vehicle sent to repair");
        check(base.remove(car), "vehicle can be decommissioned");
        check(base.getVehicleCount() == 0, "removal updates vehicle count");
    }

    private static void testChartRendering() {
        Curve curve = new Curve("line", "y=x", "black",
                List.of(new SamplePoint(0, 0), new SamplePoint(1, 1)));
        Chart chart = new Chart("Test",
                new Axis(Axis.Direction.HORIZONTAL, "x", 0, 1),
                new Axis(Axis.Direction.VERTICAL, "y", 0, 1),
                new Grid(2, 2), List.of(curve));
        check(chart.render().contains("Curve 'line'"), "chart renders its curves");
        check(chart.render().contains("Legend:"), "chart renders its legend");
    }

    private static Engine engine() {
        return new Engine("TEST-1", 100, 1.6, FuelType.PETROL, 4);
    }

    private static void check(boolean condition, String description) {
        if (!condition) {
            throw new AssertionError(description);
        }
    }

    private static void checkClose(double actual, double expected, String description) {
        if (Math.abs(actual - expected) > EPSILON) {
            throw new AssertionError(description + ": expected " + expected + ", got " + actual);
        }
    }

    private static void expect(Class<? extends Throwable> expectedType, Runnable action,
                               String description) {
        try {
            action.run();
        } catch (Throwable actual) {
            if (expectedType.isInstance(actual)) {
                return;
            }
            throw new AssertionError(description + ": unexpected exception", actual);
        }
        throw new AssertionError(description + ": expected " + expectedType.getSimpleName());
    }
}

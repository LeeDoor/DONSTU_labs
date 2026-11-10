package lab4;

import lab4.car.Bus;
import lab4.car.Engine;
import lab4.car.FleetBase;
import lab4.car.FuelType;
import lab4.car.PassengerCar;
import lab4.car.SpecialCar;
import lab4.car.Truck;
import lab4.complex.Complex;
import lab4.complex.ComplexFunctions;
import lab4.graph.Axis;
import lab4.graph.Chart;
import lab4.graph.Curve;
import lab4.graph.Grid;
import lab4.graph.SamplePoint;

import java.util.List;

public final class Lab4 {
    private Lab4() {
    }

    public static void main(String[] args) {
        if (args.length > 1) {
            System.out.println("Usage: ./run.sh [task-number 1..9]");
            return;
        }
        if (args.length == 1) {
            int task;
            try {
                task = Integer.parseInt(args[0]);
            } catch (NumberFormatException exception) {
                System.out.println("Task number must be an integer from 1 to 9.");
                return;
            }
            if (task < 1 || task > 9) {
                System.out.println("Task number must be from 1 to 9.");
                return;
            }
            runTask(task);
            return;
        }

        for (int task = 1; task <= 9; task++) {
            runTask(task);
            System.out.println();
        }
    }

    private static void runTask(int task) {
        switch (task) {
            case 1 -> showCar();
            case 2 -> showComplexNumbers();
            case 3 -> showComplexFunctions();
            case 4 -> showEngine();
            case 5 -> showVehicleTypes();
            case 6 -> showInheritanceDesign();
            case 7 -> showFleetBase();
            case 8 -> showUmlReference();
            case 9 -> showChart();
            default -> throw new IllegalArgumentException("Unknown task: " + task);
        }
    }

    private static Engine petrolEngine() {
        return new Engine("ENG-2026-001", 150, 2.0, FuelType.PETROL, 4);
    }

    private static void showCar() {
        System.out.println("Task 1. Car properties and registration validation");
        PassengerCar car = new PassengerCar(
                "Lada", "white", petrolEngine(), 4, "А 123 ВС 61 RUS");
        car.setColor("blue");
        System.out.println(car.describe());
        System.out.println("Invalid plate rejected: "
                + !car.trySetRegistrationNumber("INVALID"));
    }

    private static void showComplexNumbers() {
        System.out.println("Task 2. Complex number operations");
        Complex first = new Complex(3, 2);
        Complex second = new Complex(1, -4);
        System.out.println("z1 = " + first);
        System.out.println("z2 = " + second);
        System.out.println("z1 + z2 = " + first.add(second));
        System.out.println("z1 * z2 = " + first.multiply(second));
        System.out.println("z1 / z2 = " + first.divide(second));
        System.out.println("conjugate(z1) = " + first.conjugate());
        System.out.println("trigonometric form = " + first.toTrigonometricString());
    }

    private static void showComplexFunctions() {
        System.out.println("Task 3. Static complex functions");
        Complex value = new Complex(1, 0.5);
        System.out.println("z = " + value);
        System.out.println("exp(z) = " + ComplexFunctions.exp(value));
        System.out.println("sin(z) = " + ComplexFunctions.sin(value));
        System.out.println("cos(z) = " + ComplexFunctions.cos(value));
        System.out.println("tan(z) = " + ComplexFunctions.tan(value));
        System.out.println("atan(z) = " + ComplexFunctions.atan(value));
        System.out.println("sinh(z) = " + ComplexFunctions.sinh(value));
        System.out.println("cosh(z) = " + ComplexFunctions.cosh(value));
        System.out.println("tanh(z) = " + ComplexFunctions.tanh(value));
        System.out.println("coth(z) = " + ComplexFunctions.coth(value));
    }

    private static void showEngine() {
        System.out.println("Task 4. Engine composition");
        System.out.println(petrolEngine());
    }

    private static void showVehicleTypes() {
        System.out.println("Task 5. Specialized vehicle classes");
        System.out.println(new PassengerCar("Lada", "white", petrolEngine(), 4, "А 123 ВС 61 RUS")
                .describe());
        System.out.println(new Truck("KAMAZ", "orange", petrolEngine(), 6, 12.5,
                "В 456 КМ 161 RUS").describe());
        System.out.println(new Bus("PAZ", "yellow", petrolEngine(), 4, 30,
                "Е 789 НО 61 RUS").describe());
        System.out.println(new SpecialCar("UAZ", "red", petrolEngine(), 4, "fire service",
                "SP-0001").describe());
    }

    private static void showInheritanceDesign() {
        System.out.println("Task 6. Inheritance constraints");
        System.out.println("Car is abstract; immutable common accessors are final; Bus is final.");
    }

    private static void showFleetBase() {
        System.out.println("Task 7. Fleet base and vehicle states");
        PassengerCar passengerCar = new PassengerCar(
                "Lada", "white", petrolEngine(), 4, "А 123 ВС 61 RUS");
        Truck truck = new Truck("KAMAZ", "orange", petrolEngine(), 6, 12.5,
                "В 456 КМ 161 RUS");
        FleetBase base = new FleetBase(3);
        base.add(passengerCar);
        base.add(truck);
        base.dispatchToTrip(truck);
        System.out.println("Ready on base: " + base.listReadyOnBase().size());
        System.out.println("In trip: " + base.listInTrip().size());
        base.returnToBase(truck);
        base.sendToRepair(passengerCar);
        System.out.println("In repair: " + base.listInRepair().size());
    }

    private static void showUmlReference() {
        System.out.println("Task 8. UML class diagram: class-diagram.puml");
        System.out.println("Task 9. Chart rendering uses the graph model.");
    }

    private static void showChart() {
        System.out.println("Task 9. Text rendering of a function chart");
        Curve sine = new Curve("sine", "sin(x)", "blue", List.of(
                new SamplePoint(0, 0),
                new SamplePoint(Math.PI / 2, 1),
                new SamplePoint(Math.PI, 0)));
        Curve cosine = new Curve("cosine", "cos(x)", "red", List.of(
                new SamplePoint(0, 1),
                new SamplePoint(Math.PI / 2, 0),
                new SamplePoint(Math.PI, -1)));
        Chart chart = new Chart("Trigonometric functions",
                new Axis(Axis.Direction.HORIZONTAL, "x", 0, Math.PI),
                new Axis(Axis.Direction.VERTICAL, "y", -1, 1),
                new Grid(10, 8),
                List.of(sine, cosine));
        System.out.println(chart.render());
    }
}

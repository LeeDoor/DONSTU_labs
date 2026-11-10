package lab4.complex;

import java.util.Objects;
import java.util.Locale;

public final class Complex {
    private final double real;
    private final double imaginary;

    public Complex(double real, double imaginary) {
        if (!Double.isFinite(real) || !Double.isFinite(imaginary)) {
            throw new IllegalArgumentException("Complex components must be finite.");
        }
        this.real = real;
        this.imaginary = imaginary;
    }

    public double getReal() {
        return real;
    }

    public double getImaginary() {
        return imaginary;
    }

    public double modulus() {
        return Math.hypot(real, imaginary);
    }

    public double argument() {
        return Math.atan2(imaginary, real);
    }

    public Complex add(Complex other) {
        Objects.requireNonNull(other, "Operand must not be null.");
        return new Complex(real + other.real, imaginary + other.imaginary);
    }

    public Complex add(double value) {
        return add(new Complex(value, 0));
    }

    public Complex subtract(Complex other) {
        Objects.requireNonNull(other, "Operand must not be null.");
        return new Complex(real - other.real, imaginary - other.imaginary);
    }

    public Complex subtract(double value) {
        return subtract(new Complex(value, 0));
    }

    public Complex multiply(Complex other) {
        Objects.requireNonNull(other, "Operand must not be null.");
        return new Complex(
                real * other.real - imaginary * other.imaginary,
                real * other.imaginary + imaginary * other.real);
    }

    public Complex multiply(double value) {
        return new Complex(real * value, imaginary * value);
    }

    public Complex divide(Complex other) {
        Objects.requireNonNull(other, "Operand must not be null.");
        double denominator = other.real * other.real + other.imaginary * other.imaginary;
        if (denominator == 0) {
            throw new ArithmeticException("Division by zero complex number.");
        }
        return new Complex(
                (real * other.real + imaginary * other.imaginary) / denominator,
                (imaginary * other.real - real * other.imaginary) / denominator);
    }

    public Complex divide(double value) {
        if (value == 0) {
            throw new ArithmeticException("Division by zero.");
        }
        return new Complex(real / value, imaginary / value);
    }

    public Complex conjugate() {
        return new Complex(real, -imaginary);
    }

    public String toTrigonometricString() {
        return String.format(Locale.ROOT, "%.6g * (cos(%.6g) + i*sin(%.6g))",
                modulus(), argument(), argument());
    }

    @Override
    public String toString() {
        if (imaginary == 0) {
            return String.format(Locale.ROOT, "%.6g", real);
        }
        if (real == 0) {
            return String.format(Locale.ROOT, "%.6gi", imaginary);
        }
        return String.format(Locale.ROOT, "%.6g %s %.6gi",
                real, imaginary < 0 ? "-" : "+", Math.abs(imaginary));
    }

    @Override
    public boolean equals(Object other) {
        if (this == other) {
            return true;
        }
        if (!(other instanceof Complex)) {
            return false;
        }
        Complex that = (Complex) other;
        return real == that.real && imaginary == that.imaginary;
    }

    @Override
    public int hashCode() {
        return Objects.hash(real == 0 ? 0.0 : real, imaginary == 0 ? 0.0 : imaginary);
    }
}

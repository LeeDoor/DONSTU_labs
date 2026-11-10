package lab4.complex;

import java.util.Objects;

public final class ComplexFunctions {
    private static final Complex ONE = new Complex(1, 0);
    private static final Complex I = new Complex(0, 1);

    private ComplexFunctions() {
    }

    public static Complex exp(Complex value) {
        Objects.requireNonNull(value, "Argument must not be null.");
        double scale = Math.exp(value.getReal());
        return new Complex(
                scale * Math.cos(value.getImaginary()),
                scale * Math.sin(value.getImaginary()));
    }

    public static Complex sin(Complex value) {
        Complex iz = I.multiply(Objects.requireNonNull(value, "Argument must not be null."));
        return exp(iz).subtract(exp(iz.multiply(-1))).divide(I.multiply(2));
    }

    public static Complex cos(Complex value) {
        Complex iz = I.multiply(Objects.requireNonNull(value, "Argument must not be null."));
        return exp(iz).add(exp(iz.multiply(-1))).divide(2);
    }

    public static Complex tan(Complex value) {
        return sin(value).divide(cos(value));
    }

    public static Complex atan(Complex value) {
        Objects.requireNonNull(value, "Argument must not be null.");
        Complex iz = I.multiply(value);
        Complex ratio = ONE.add(iz).divide(ONE.subtract(iz));
        Complex logarithm = new Complex(Math.log(ratio.modulus()), ratio.argument());
        return logarithm.multiply(I.multiply(-0.5));
    }

    public static Complex sinh(Complex value) {
        Objects.requireNonNull(value, "Argument must not be null.");
        return exp(value).subtract(exp(value.multiply(-1))).divide(2);
    }

    public static Complex cosh(Complex value) {
        Objects.requireNonNull(value, "Argument must not be null.");
        return exp(value).add(exp(value.multiply(-1))).divide(2);
    }

    public static Complex tanh(Complex value) {
        return sinh(value).divide(cosh(value));
    }

    public static Complex coth(Complex value) {
        return cosh(value).divide(sinh(value));
    }
}

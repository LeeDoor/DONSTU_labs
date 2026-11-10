package lab4.graph;

public final class Axis implements ChartElement {
    public enum Direction {
        HORIZONTAL,
        VERTICAL
    }

    private final Direction direction;
    private final String label;
    private final double minimum;
    private final double maximum;

    public Axis(Direction direction, String label, double minimum, double maximum) {
        if (direction == null) {
            throw new IllegalArgumentException("Axis direction must not be null.");
        }
        if (label == null || label.isBlank()) {
            throw new IllegalArgumentException("Axis label must not be blank.");
        }
        if (!Double.isFinite(minimum) || !Double.isFinite(maximum) || minimum >= maximum) {
            throw new IllegalArgumentException("Axis range must be finite and increasing.");
        }
        this.direction = direction;
        this.label = label.trim();
        this.minimum = minimum;
        this.maximum = maximum;
    }

    public Direction getDirection() {
        return direction;
    }

    public String getLabel() {
        return label;
    }

    public double getMinimum() {
        return minimum;
    }

    public double getMaximum() {
        return maximum;
    }

    @Override
    public String render() {
        return String.format("%s axis '%s' [%g, %g]", direction, label, minimum, maximum);
    }
}

package lab4.graph;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class Curve implements ChartElement {
    private final String name;
    private final String expression;
    private final String color;
    private final List<SamplePoint> points;

    public Curve(String name, String expression, String color, List<SamplePoint> points) {
        if (name == null || name.isBlank() || expression == null || expression.isBlank()
                || color == null || color.isBlank()) {
            throw new IllegalArgumentException("Curve name, expression and color are required.");
        }
        if (points == null || points.isEmpty()) {
            throw new IllegalArgumentException("A curve must contain at least one sample point.");
        }
        this.name = name.trim();
        this.expression = expression.trim();
        this.color = color.trim();
        this.points = Collections.unmodifiableList(new ArrayList<>(points));
        if (this.points.contains(null)) {
            throw new IllegalArgumentException("Curve points must not contain null.");
        }
    }

    public String getName() {
        return name;
    }

    public String getExpression() {
        return expression;
    }

    public String getColor() {
        return color;
    }

    public List<SamplePoint> getPoints() {
        return points;
    }

    @Override
    public String render() {
        return String.format("Curve '%s': %s, color=%s, %d sample points",
                name, expression, color, points.size());
    }
}

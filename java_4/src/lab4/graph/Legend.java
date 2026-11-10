package lab4.graph;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class Legend implements ChartElement {
    private final List<Curve> curves;

    public Legend(List<Curve> curves) {
        if (curves == null) {
            throw new IllegalArgumentException("Legend curves must not be null.");
        }
        this.curves = Collections.unmodifiableList(new ArrayList<>(curves));
        if (this.curves.contains(null)) {
            throw new IllegalArgumentException("Legend curves must not contain null.");
        }
    }

    @Override
    public String render() {
        StringBuilder result = new StringBuilder("Legend:");
        for (Curve curve : curves) {
            result.append(System.lineSeparator())
                    .append("  ").append(curve.getName()).append(" (")
                    .append(curve.getColor()).append(")");
        }
        return result.toString();
    }
}

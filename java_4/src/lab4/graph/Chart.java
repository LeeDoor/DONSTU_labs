package lab4.graph;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public final class Chart implements ChartElement {
    private final String title;
    private final Axis horizontalAxis;
    private final Axis verticalAxis;
    private final Grid grid;
    private final List<Curve> curves;
    private final Legend legend;

    public Chart(String title, Axis horizontalAxis, Axis verticalAxis,
                 Grid grid, List<Curve> curves) {
        if (title == null || title.isBlank()) {
            throw new IllegalArgumentException("Chart title must not be blank.");
        }
        if (horizontalAxis == null || verticalAxis == null || grid == null || curves == null) {
            throw new IllegalArgumentException("Chart axes, grid and curves must not be null.");
        }
        if (horizontalAxis.getDirection() != Axis.Direction.HORIZONTAL
                || verticalAxis.getDirection() != Axis.Direction.VERTICAL) {
            throw new IllegalArgumentException("Chart axes have incorrect directions.");
        }
        this.title = title.trim();
        this.horizontalAxis = horizontalAxis;
        this.verticalAxis = verticalAxis;
        this.grid = grid;
        this.curves = Collections.unmodifiableList(new ArrayList<>(curves));
        if (this.curves.contains(null)) {
            throw new IllegalArgumentException("Chart curves must not contain null.");
        }
        this.legend = new Legend(this.curves);
    }

    public String getTitle() {
        return title;
    }

    public List<Curve> getCurves() {
        return curves;
    }

    @Override
    public String render() {
        StringBuilder result = new StringBuilder("Chart: ").append(title)
                .append(System.lineSeparator()).append(horizontalAxis.render())
                .append(System.lineSeparator()).append(verticalAxis.render())
                .append(System.lineSeparator()).append(grid.render());
        for (Curve curve : curves) {
            result.append(System.lineSeparator()).append(curve.render());
        }
        return result.append(System.lineSeparator()).append(legend.render()).toString();
    }
}

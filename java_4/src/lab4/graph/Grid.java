package lab4.graph;

public final class Grid implements ChartElement {
    private final int verticalLineCount;
    private final int horizontalLineCount;

    public Grid(int verticalLineCount, int horizontalLineCount) {
        if (verticalLineCount < 0 || horizontalLineCount < 0) {
            throw new IllegalArgumentException("Grid line counts must not be negative.");
        }
        this.verticalLineCount = verticalLineCount;
        this.horizontalLineCount = horizontalLineCount;
    }

    @Override
    public String render() {
        return String.format("Grid: %d vertical and %d horizontal lines",
                verticalLineCount, horizontalLineCount);
    }
}

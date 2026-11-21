import java.awt.*;
import javax.swing.*;

class Curve {
    private double[] x;
    private double[] y;
    private int left;
    private int top;
    private int width;
    private int height;

    public void setData(double xMin, double xMax, int count) {
        x = new double[count];
        y = new double[count];

        for (int i = 0; i < count; i++) {
            x[i] = xMin + i * (xMax - xMin) / (count - 1);
            y[i] = Math.sin(x[i]);
        }
    }

    public void setRect(int left, int top, int width, int height) {
        this.left = left;
        this.top = top;
        this.width = width;
        this.height = height;
    }

    public void draw(Graphics g) {
        if (x == null || x.length < 2) {
            return;
        }

        g.setColor(Color.BLACK);
        g.drawRect(left, top, width, height);
        g.drawLine(left, top + height / 2, left + width, top + height / 2);

        g.setColor(Color.BLUE);

        for (int i = 0; i < x.length - 1; i++) {
            int x1 = left + (int) Math.round((x[i] - x[0]) / (x[x.length - 1] - x[0]) * width);
            int x2 = left + (int) Math.round((x[i + 1] - x[0]) / (x[x.length - 1] - x[0]) * width);

            int y1 = top + height / 2 - (int) Math.round(y[i] * height / 2.0);
            int y2 = top + height / 2 - (int) Math.round(y[i + 1] * height / 2.0);

            g.drawLine(x1, y1, x2, y2);
        }
    }
}

public class Main extends JPanel {
    private Curve curve;

    public Main() {
        curve = new Curve();
        curve.setData(0, 2 * Math.PI, 501);
        curve.setRect(25, 20, 240, 240);

        JFrame frame = new JFrame("Task 2 - Curve");
        frame.setSize(300, 300);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(this);
        frame.setVisible(true);
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);
        curve.draw(g);
    }

    public static void main(String[] args) {
        new Main();
    }
}

import java.awt.*;
import java.util.ArrayList;
import javax.swing.*;

class Curve {
    private double[] x;
    private double[] y;

    public Curve(double xMin, double xMax, int count) {
        x = new double[count];
        y = new double[count];

        for (int i = 0; i < count; i++) {
            x[i] = xMin + i * (xMax - xMin) / (count - 1);
            y[i] = Math.sin(x[i]);
        }
    }

    public double[] getX() {
        return x;
    }

    public double[] getY() {
        return y;
    }
}

class Axis {
    public void draw(Graphics g, int left, int top, int width, int height) {
        g.setColor(Color.BLACK);
        g.drawLine(left, top + height / 2, left + width, top + height / 2);
        g.drawLine(left, top, left, top + height);
        g.drawString("X", left + width - 10, top + height / 2 - 5);
        g.drawString("Y", left + 5, top + 10);
    }
}

class Graph extends JPanel {
    private ArrayList<Curve> curves;
    private Axis axis;

    public Graph() {
        curves = new ArrayList<Curve>();
        axis = new Axis();
        curves.add(new Curve(-Math.PI, Math.PI, 501));
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);

        int left = 35;
        int top = 20;
        int width = getWidth() - 55;
        int height = getHeight() - 55;

        axis.draw(g, left, top, width, height);

        for (int k = 0; k < curves.size(); k++) {
            double[] x = curves.get(k).getX();
            double[] y = curves.get(k).getY();

            g.setColor(Color.RED);

            for (int i = 0; i < x.length - 1; i++) {
                int x1 = left + (int) Math.round((x[i] + Math.PI) / (2 * Math.PI) * width);
                int x2 = left + (int) Math.round((x[i + 1] + Math.PI) / (2 * Math.PI) * width);
                int y1 = top + height / 2 - (int) Math.round(y[i] * height / 2.0);
                int y2 = top + height / 2 - (int) Math.round(y[i + 1] * height / 2.0);
                g.drawLine(x1, y1, x2, y2);
            }
        }
    }
}

public class Main {
    public static void main(String[] args) {
        JFrame frame = new JFrame("Task 3 - Graph, Axis, Curve");
        frame.setSize(300, 300);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(new Graph());
        frame.setVisible(true);
    }
}

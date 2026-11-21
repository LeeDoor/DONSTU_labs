import java.awt.*;
import javax.swing.*;

public class Main extends JPanel {
    public Main() {
        JFrame frame = new JFrame("Task 1 - sin(x)");
        frame.setSize(300, 300);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(this);
        frame.setVisible(true);
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);

        int w = getWidth();
        int h = getHeight();
        int left = 30;
        int right = w - 15;
        int top = 15;
        int bottom = h - 30;

        g.setColor(Color.BLACK);
        g.drawLine(left, top, left, bottom);
        g.drawLine(left, (top + bottom) / 2, right, (top + bottom) / 2);

        g.drawString("-pi", left, bottom + 18);
        g.drawString("0", (left + right) / 2, bottom + 18);
        g.drawString("pi", right - 10, bottom + 18);
        g.drawString("1", left - 15, top + 5);
        g.drawString("-1", left - 20, bottom);

        g.setColor(Color.RED);
        int previousX = 0;
        int previousY = 0;

        for (int i = 0; i <= 500; i++) {
            double x = -Math.PI + i * 2.0 * Math.PI / 500;
            double y = Math.sin(x);

            int screenX = left + (int) Math.round((x + Math.PI) / (2 * Math.PI) * (right - left));
            int screenY = (top + bottom) / 2 - (int) Math.round(y * (bottom - top) / 2.0);

            if (i > 0) {
                g.drawLine(previousX, previousY, screenX, screenY);
            }

            previousX = screenX;
            previousY = screenY;
        }
    }

    public static void main(String[] args) {
        new Main();
    }
}

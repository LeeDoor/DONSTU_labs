import java.awt.*;
import java.awt.event.*;
import java.util.ArrayList;
import java.util.Random;
import javax.swing.*;

class Ball {
    int x;
    int y;
    int radius;
    double dx;
    double dy;
    Color color;

    public Ball(int x, int y) {
        Random random = new Random();
        radius = 15;
        double speed = 2 + random.nextDouble() * 4;
        dx = speed * Math.cos(Math.PI / 4);
        dy = -speed * Math.sin(Math.PI / 4);
        color = new Color(random.nextInt(256), random.nextInt(256), random.nextInt(256));
        this.x = x;
        this.y = y;
    }

    public void move(int width, int height) {
        x += (int) Math.round(dx);
        y += (int) Math.round(dy);

        if (x - radius < 0 || x + radius > width) {
            dx = -dx;
        }

        if (y - radius < 0 || y + radius > height) {
            dy = -dy;
        }

        if (x - radius < 0) x = radius;
        if (x + radius > width) x = width - radius;
        if (y - radius < 0) y = radius;
        if (y + radius > height) y = height - radius;
    }

    public void draw(Graphics g) {
        g.setColor(color);
        g.fillOval(x - radius, y - radius, radius * 2, radius * 2);
    }
}

public class Main extends JPanel {
    private ArrayList<Ball> balls;
    private Timer timer;

    public Main() {
        balls = new ArrayList<Ball>();

        JFrame frame = new JFrame("Task 7 - Balls");
        frame.setSize(500, 400);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(this);

        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                balls.add(new Ball(e.getX(), e.getY()));
                repaint();
            }
        });

        timer = new Timer(20, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                for (int i = 0; i < balls.size(); i++) {
                    balls.get(i).move(getWidth(), getHeight());
                }
                repaint();
            }
        });

        frame.setVisible(true);
        timer.start();
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);

        for (int i = 0; i < balls.size(); i++) {
            balls.get(i).draw(g);
        }
    }

    public static void main(String[] args) {
        new Main();
    }
}

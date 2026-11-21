import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class Main extends JPanel {
    private int mouseX = -1;
    private int mouseY = -1;
    private Color textColor = Color.BLACK;

    public Main() {
        JFrame frame = new JFrame("Task 4 - Mouse and keyboard");
        frame.setSize(400, 300);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(this);

        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                mouseX = e.getX();
                mouseY = e.getY();
                repaint();
            }
        });

        addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                char key = Character.toLowerCase(e.getKeyChar());

                if (key == 'b') textColor = Color.BLACK;
                if (key == 'w') textColor = Color.WHITE;
                if (key == 'r') textColor = Color.RED;
                if (key == 'g') textColor = Color.GREEN;
                if (key == 'o') textColor = Color.ORANGE;

                repaint();
            }
        });

        setFocusable(true);
        frame.setVisible(true);
        requestFocusInWindow();
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);

        if (mouseX >= 0 && mouseY >= 0) {
            g.setColor(textColor);
            g.drawString("(" + mouseX + ", " + mouseY + ")", mouseX, mouseY);
        }
    }

    public static void main(String[] args) {
        new Main();
    }
}

import java.awt.*;
import java.awt.event.*;
import java.util.Random;
import javax.swing.*;

public class Main extends JPanel {
    private String[] messages = {
        "я@ты@я@ты@я@ты@",
        "АБОБА",
        "очень длинная строка очень правда балин буду",
        "тетрагидропиранилциклопентилтетрагидропиридопиридиновый"
    };

    private int messageIndex = 0;
    private int x = 0;
    private Timer timer;
    private Random random = new Random();

    public Main() {
        JFrame frame = new JFrame("Task 8 - Running line");
        frame.setSize(500, 200);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(this);

        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                int oldIndex = messageIndex;

                if (messages.length > 1) {
                    while (messageIndex == oldIndex) {
                        messageIndex = random.nextInt(messages.length);
                    }
                }

                x = 0;
                repaint();
            }
        });

        timer = new Timer(20, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                x += 2;

                FontMetrics metrics = getFontMetrics(getFont());
                int textWidth = metrics.stringWidth(messages[messageIndex]);

                if (x > getWidth()) {
                    x = -textWidth;
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

        g.setColor(Color.BLUE);
        g.drawString(messages[messageIndex], x, getHeight() / 2);
    }

    public static void main(String[] args) {
        new Main();
    }
}

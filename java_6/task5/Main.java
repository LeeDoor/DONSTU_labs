import java.awt.*;
import java.awt.event.*;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.*;

public class Main extends JPanel {
    private BufferedImage image;
    private boolean rotated = false;

    public Main() {
        try {
            image = ImageIO.read(new File("image.jpg"));
        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, "Put image.jpg in the program folder.");
            System.exit(1);
        }

        JFrame frame = new JFrame("Task 5 - Image");
        frame.setSize(600, 450);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(this);

        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                rotated = !rotated;
                repaint();
            }
        });

        frame.setVisible(true);
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);

        int panelWidth = getWidth();
        int panelHeight = getHeight();

        double scale = Math.min((double) panelWidth / image.getWidth(),
                                (double) panelHeight / image.getHeight());

        int drawWidth = (int) (image.getWidth() * scale);
        int drawHeight = (int) (image.getHeight() * scale);
        int x = (panelWidth - drawWidth) / 2;
        int y = (panelHeight - drawHeight) / 2;

        Graphics2D g2 = (Graphics2D) g;

        if (rotated) {
            AffineTransform transform = new AffineTransform();
            transform.translate(x + drawWidth / 2.0, y + drawHeight / 2.0);
            transform.rotate(Math.PI);
            transform.scale((double) drawWidth / image.getWidth(),
                            (double) drawHeight / image.getHeight());
            transform.translate(-image.getWidth() / 2.0, -image.getHeight() / 2.0);
            g2.drawImage(image, transform, this);
        } else {
            g2.drawImage(image, x, y, drawWidth, drawHeight, this);
        }
    }

    public static void main(String[] args) {
        new Main();
    }
}

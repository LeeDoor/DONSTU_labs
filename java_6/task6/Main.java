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
    private char selectedFilter = ' ';
    private boolean grayWasUsed = false;

    public Main() {
        try {
            image = ImageIO.read(new File("image.jpg"));
        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, "Put image.jpg in the program folder.");
            System.exit(1);
        }

        JFrame frame = new JFrame("Task 6 - Image filters");
        frame.setSize(600, 450);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(this);

        addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                char key = Character.toLowerCase(e.getKeyChar());

                if (key == 'r' || key == 'g' || key == 'b') {
                    selectedFilter = key;
                    repaint();
                }
            }
        });

        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                applyFilter();
                repaint();
            }
        });

        setFocusable(true);
        frame.setVisible(true);
        requestFocusInWindow();
    }

    private void applyFilter() {
        if (selectedFilter == 'r') {
            BufferedImage result = new BufferedImage(image.getWidth(), image.getHeight(),
                                                      BufferedImage.TYPE_INT_RGB);

            for (int y = 0; y < image.getHeight(); y++) {
                for (int x = 0; x < image.getWidth(); x++) {
                    result.setRGB(image.getWidth() - 1 - x,
                                  image.getHeight() - 1 - y, image.getRGB(x, y));
                }
            }

            image = result;
        }

        if (selectedFilter == 'g' && !grayWasUsed) {
            for (int y = 0; y < image.getHeight(); y++) {
                for (int x = 0; x < image.getWidth(); x++) {
                    Color c = new Color(image.getRGB(x, y));
                    int average = (c.getRed() + c.getGreen() + c.getBlue()) / 3;
                    image.setRGB(x, y, new Color(average, average, average).getRGB());
                }
            }

            grayWasUsed = true;
        }

        if (selectedFilter == 'b') {
            BufferedImage source = new BufferedImage(image.getWidth(), image.getHeight(),
                                                      BufferedImage.TYPE_INT_RGB);
            Graphics g = source.getGraphics();
            g.drawImage(image, 0, 0, null);
            g.dispose();

            for (int y = 1; y < image.getHeight() - 1; y++) {
                for (int x = 1; x < image.getWidth() - 1; x++) {
                    int red = 0;
                    int green = 0;
                    int blue = 0;

                    for (int dy = -1; dy <= 1; dy++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            Color c = new Color(source.getRGB(x + dx, y + dy));
                            red += c.getRed();
                            green += c.getGreen();
                            blue += c.getBlue();
                        }
                    }

                    red /= 9;
                    green /= 9;
                    blue /= 9;
                    image.setRGB(x, y, new Color(red, green, blue).getRGB());
                }
            }
        }
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);

        double scale = Math.min((double) getWidth() / image.getWidth(),
                                (double) getHeight() / image.getHeight());

        int width = (int) (image.getWidth() * scale);
        int height = (int) (image.getHeight() * scale);
        int x = (getWidth() - width) / 2;
        int y = (getHeight() - height) / 2;

        g.drawImage(image, x, y, width, height, this);
        g.setColor(Color.BLACK);
        g.drawString("Selected filter: " + selectedFilter + " (click to apply)", 10, 20);
    }

    public static void main(String[] args) {
        new Main();
    }
}

package raytracer;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.Timer;
import raytracer.materials.Lambertian;
import raytracer.materials.Metal;
import raytracer.objects.ObjectGroup;
import raytracer.objects.Sphere;

public class Raytracer extends JPanel {

//    static int width = 200;
//    static int height = 100;
//    static int zoom = 4;
    
    static int width = 1000;
    static int height = 500;
    static int zoom = 1;

    static int gamma = 2;
    
    public static Color[][] screen = new Color[width][height];
    
    @Override
    public void paint(Graphics g) {
        super.paint(g);
        Graphics2D g2d = (Graphics2D)g;

        for(int i=0 ; i<width ; i++)
            for(int j=0 ; j<height ; j++) {
                
                if(screen[i][j] == null)
                    break;
                
                g2d.setColor(screen[i][j]);
                g2d.fillRect(i*zoom, (height-j-1)*zoom, zoom, zoom);
            }
    }
    
    public static void main(String[] args) throws InterruptedException {
        JFrame frame = new JFrame();
        Raytracer raytracer = new Raytracer();
        raytracer.setPreferredSize(new Dimension(width*zoom, height*zoom));
        frame.add(raytracer);
        frame.pack();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);

        Timer refresh = new Timer(1000, (ActionEvent e) -> { frame.repaint(); });
        refresh.start();
        
        
        Camera camera = new Camera(width, height, new Vec(0f, 0f, 1f), new Vec(-2f, -1f, -1f), new Vec(4f, 0f, 0f), new Vec(0f, 2f, 0f));
        
        ObjectGroup world = new ObjectGroup();
        world.add(new Sphere(new Vec(0f, -1000.5f, -1f), 1000f, new Lambertian(new Vec(0.8f, 0.8f, 0f))));
//        world.add(new Sphere(new Vec(0f, 0f, -1f), 0.5f, new Lambertian(new Vec(0.8f, 0.3f, 0.3f))));
//        world.add(new Sphere(new Vec(1.2f, 0f, -3f), 0.5f, new Metal(new Vec(0.8f, 0.6f, 0.2f))));
//        world.add(new Sphere(new Vec(-1.5f, 0f, -2f), 0.5f, new Metal(new Vec(0.8f, 0.8f, 0.8f))));
//        world.add(new Sphere(new Vec(0f, 5f, 0f), 0.5f, new Lambertian(new Vec(0.1f, 0.1f, 0.8f))));
        
        for(int i=0 ; i<30 ; i++)
            world.add(new Sphere(new Vec(5f-10f*(float)Math.random(), 0f, -8f*(float)Math.random()),
                    0.3f + 0.5f*(float)Math.random(),
                    Math.random() > 0.2 ?
                              new Lambertian(new Vec((float)Math.random(), (float)Math.random(), (float)Math.random()))
                            : new Metal(     new Vec((float)Math.random(), (float)Math.random(), (float)Math.random()))));

        for(int i=0 ; i<width ; i++)
            for(int j=0 ; j<height ; j++)
                screen[i][j] = camera.getColorWithAntialiasing(world, i, j, 100, 100).toRGB(gamma);
    }
}

package raytracer;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import javax.swing.JFrame;
import javax.swing.JPanel;
import raytracer.materials.Lambertian;
import raytracer.materials.Metal;
import raytracer.objects.ObjectGroup;
import raytracer.objects.Sphere;

public class Raytracer extends JPanel {

    static int width = 200;
    static int height = 100;
    static int zoom = 1;
    
    static int gamma = 3;
    
    @Override
    public void paint(Graphics g) {
        super.paint(g);
        Graphics2D g2d = (Graphics2D)g;
        
        Camera camera = new Camera(width, height, new Vec(0f, 0f, 0f), new Vec(-2f, -1f, -1f), new Vec(4f, 0f, 0f), new Vec(0f, 2f, 0f));
        
        ObjectGroup world = new ObjectGroup();
        world.add(new Sphere(new Vec(0f, 0f, -1f), 0.5f, new Lambertian(new Vec(0.8f, 0.3f, 0.3f))));
        world.add(new Sphere(new Vec(0f, -100.5f, -1f), 100f, new Lambertian(new Vec(0.8f, 0.8f, 0f))));
        world.add(new Sphere(new Vec(1f, 0f, -1f), 0.5f, new Metal(new Vec(0.8f, 0.6f, 0.2f))));
        world.add(new Sphere(new Vec(-1f, 0f, -1f), 0.5f, new Metal(new Vec(0.8f, 0.8f, 0.8f))));
        
        for(int i=0 ; i<width ; i++)
            for(int j=0 ; j<height ; j++){
                g2d.setColor(camera.getColorWithAntialiasing(world, i, j, 100, 1000).toRGB(gamma));
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
    }
}

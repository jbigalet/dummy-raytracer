package raytracer;

import raytracer.objects.HittableObject;

public class Camera {

    public int width;
    public int height;
    public Vec orig;
    public Vec lower_left_corner;
    public Vec hor;
    public Vec vert;

    public Camera(int width, int height, Vec orig, Vec lower_left_corner, Vec hor, Vec vert) {
        this.width = width;
        this.height = height;
        this.orig = orig;
        this.lower_left_corner = lower_left_corner;
        this.hor = hor;
        this.vert = vert;
    }
    
    public Ray getRay(float u, float v) {
        return new Ray(orig, lower_left_corner.add(hor.mul(u)).add(vert.mul(v)).minus(orig));
    }
    
    public Vec getColor(HittableObject obj, int x, int y, int max_bounce){
        return getRay(x/(float)width, y/(float)height).color(obj, max_bounce);
    }

    public Vec getColorWithAntialiasing(HittableObject obj, int x, int y, int max_bounce, int nsamples){
        Vec col = new Vec(0f, 0f, 0f);
        for(int i=0 ; i<nsamples ; i++)
            col = col.add( getRay((float)(x+Math.random())/(float)width, (float)(y+Math.random())/(float)height).color(obj, max_bounce) );
        return col.mul(1f/nsamples);
    }
}
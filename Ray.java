package raytracer;

import raytracer.objects.HitRecord;
import raytracer.objects.HittableObject;

public class Ray {
    
    public Vec orig;
    public Vec dir;
    
    public Vec attenuation;

    public Ray(Vec orig, Vec dir) {
        this.orig = orig;
        this.dir = dir;
        this.attenuation = new Vec(1f, 1f, 1f);
    }

    public Ray(Vec orig, Vec dir, Vec attenuation) {
        this.orig = orig;
        this.dir = dir;
        this.attenuation = attenuation;
    }
    
    public Vec point_at_parameter(float t){
        return orig.add(dir.mul(t));
    }
    
    public Vec color(HittableObject obj, int max_bounce) {
        if(max_bounce == 0)
            return new Vec(0f, 0f, 0f);
        
        HitRecord hit = obj.hit(this, 0.0001f, Float.MAX_VALUE);
        
        if( hit != null ){
            Ray scattered = hit.material.scatter(this, hit);
            if( scattered != null )
                return scattered.color(obj, max_bounce-1).mul(scattered.attenuation);
            else
                return new Vec(0f, 0f, 0f);
        }
        
        float t = 0.5f*(dir.unit().y() + 1.0f);
        return new Vec(1f, 1f, 1f).mul(1f-t).add(new Vec(0.5f, 0.7f, 1.0f).mul(t));
    }

}
package raytracer.materials;

import raytracer.objects.HitRecord;
import raytracer.Ray;
import raytracer.Vec;

public class Lambertian extends Material {

    public Vec albedo;

    public Lambertian(Vec albedo) {
        this.albedo = albedo;
    }
    
    @Override
    public Ray scatter(Ray r, HitRecord rec) {
        
        // random point in unit sphere
        Vec random_point;
        do {            
            random_point = new Vec(2f*(float)Math.random()-1f, 2f*(float)Math.random()-1f, 2f*(float)Math.random()-1f);
        } while (random_point.dot(random_point) >= 1f);
        
        return new Ray(rec.p, rec.normal.add(random_point), albedo);
    }
}

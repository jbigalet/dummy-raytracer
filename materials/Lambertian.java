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
        return new Ray(rec.p, rec.normal.add(Vec.randomPointInSphere()), albedo);
    }
}

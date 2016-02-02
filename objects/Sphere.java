package raytracer.objects;

import raytracer.Ray;
import raytracer.Vec;
import raytracer.materials.Material;

public class Sphere extends HittableObject {

    public Vec center;
    public float radius;
    public Material material;

    public Sphere(Vec center, float radius, Material material) {
        this.center = center;
        this.radius = radius;
        this.material = material;
    }
    
    @Override
    public HitRecord hit(Ray ray, float t_min, float t_max) {
        Vec oc = ray.orig.minus(center);
        float a = ray.dir.dot(ray.dir);
        float b = 2f * oc.dot(ray.dir);
        float c = oc.dot(oc) - radius*radius;
        float discriminant = b*b - 4*a*c;
        
        if(discriminant > 0) {
            float tmp = (-b-(float)Math.sqrt(discriminant))/(2f*a);
            if( tmp > t_min && tmp < t_max )
                return new HitRecord(tmp, ray.point_at_parameter(tmp), ray.point_at_parameter(tmp).minus(center).mul(1f/radius), this.material);
            
            tmp = (-b+(float)Math.sqrt(discriminant))/(2f*a);
            if( tmp > t_min && tmp < t_max )
                return new HitRecord(tmp, ray.point_at_parameter(tmp), ray.point_at_parameter(tmp).minus(center).mul(1f/radius), this.material);
        }
        
        return null;
    }
    
}

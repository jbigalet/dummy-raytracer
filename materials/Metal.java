package raytracer.materials;

import raytracer.Ray;
import raytracer.Vec;
import raytracer.objects.HitRecord;

public class Metal extends Material {
    
    public Vec albedo;

    public Metal(Vec albedo) {
        this.albedo = albedo;
    }

    @Override
    public Ray scatter(Ray r, HitRecord rec) {
        Ray res = new Ray(rec.p, r.dir.unit().reflect(rec.normal), albedo);
        if( res.dir.dot(rec.normal) > 0 )
            return res;
        return null;
    }
}

package raytracer.materials;

import raytracer.Ray;
import raytracer.objects.HitRecord;

public class Light extends Material {

    public Light() {
        this.light = true;
    }
    
    @Override
    public Ray scatter(Ray r, HitRecord rec) {
        return null;
    }
}

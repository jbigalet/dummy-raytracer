package raytracer.materials;

import raytracer.objects.HitRecord;
import raytracer.Ray;

public abstract class Material {
    public boolean light;
    abstract public Ray scatter(Ray r, HitRecord rec);
}

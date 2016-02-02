package raytracer.objects;

import raytracer.Ray;

public abstract class HittableObject {
    abstract public HitRecord hit(Ray ray, float t_min, float t_max);
}
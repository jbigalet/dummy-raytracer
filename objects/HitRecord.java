package raytracer.objects;

import raytracer.Vec;
import raytracer.materials.Material;

public class HitRecord {
    public float t;
    public Vec p;
    public Vec normal;
    
    public Material material;

    public HitRecord(float t, Vec p, Vec normal, Material material) {
        this.t = t;
        this.p = p;
        this.normal = normal;
        this.material = material;
    }

}

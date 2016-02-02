package raytracer.objects;

import java.util.ArrayList;
import raytracer.Ray;

public class ObjectGroup extends HittableObject {

    public ArrayList<HittableObject> list;

    public ObjectGroup() {
        this.list = new ArrayList<>();
    }
    
    public ObjectGroup(ArrayList<HittableObject> list) {
        this.list = list;
    }
    
    public void add(HittableObject obj){
        this.list.add(obj);
    }
    
    @Override
    public HitRecord hit(Ray ray, float t_min, float t_max) {
        HitRecord bestHit = null;
        for(HittableObject obj: list){
            HitRecord hit = obj.hit(ray, t_min, bestHit == null ? t_max : bestHit.t);
            if(hit != null)
                bestHit = hit;
        }
        return bestHit;
    }
    
}

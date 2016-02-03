package raytracer;

import java.awt.Color;

public class Vec {
    
    float[] v;

    public Vec(float a, float b, float c) {
        v = new float[] {a, b, c};
    }
    
    public static Vec randomPointInSphere() {
        // random point in unit sphere
        Vec random_point;
        do {            
            random_point = new Vec(2f*(float)Math.random()-1f, 2f*(float)Math.random()-1f, 2f*(float)Math.random()-1f);
        } while (random_point.dot(random_point) >= 1f);
        return random_point;
    }
    
    public float x() { return v[0]; }
    public float y() { return v[1]; }
    public float z() { return v[2]; }
    
    
    // operators
    
    public Vec add(Vec w){
        return new Vec(v[0]+w.x(), v[1]+w.y(), v[2]+w.z());
    }
    
    public Vec minus(Vec w){
        return new Vec(v[0]-w.x(), v[1]-w.y(), v[2]-w.z());
    }

    public Vec mul(float f){
        return new Vec(f*v[0], f*v[1], f*v[2]);
    }

    public Vec mul(Vec w){
        return new Vec(v[0]*w.x(), v[1]*w.y(), v[2]*w.z());
    }
    
    public float dot(Vec w){
        return v[0]*w.x() + v[1]*w.y() + v[2]*w.z();
    }
    
    
    // helpers
    
    public Vec reflect(Vec norm) {
        return this.minus(norm.mul(2f*this.dot(norm)));
    }
    
    public float length() {
        return (float)Math.sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    }
    
    public Vec unit() {
        return this.mul(1f / this.length());
    }
    
    public Color toRGG() {
        return toRGB(1);
    }
    
    public Color toRGB(int gamma){
        return new Color((int)(255*Math.pow(v[0], 1d/gamma)), (int)(255*Math.pow(v[1], 1d/gamma)), (int)(255*Math.pow(v[2], 1d/gamma)));
    }
    
    @Override
    public String toString() {
        return "(" + v[0] + ", " + v[1] + ", " + v[2] + ")";
    }
}

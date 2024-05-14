class Point{
    public float x;
    public float y;

    public float z;

    public Point(float x, float y, float z){
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public Point Clone(){
        return new Point(this.x, this.y, this.z);
    }
}

class Triangle{
    public Point point1;
    public Point point2;
    public Point point3;
    public Point normal;

    public Triangle(Point p1, Point p2, Point p3){

        this.point1 = p1;
        this.point2 = p2;
        this.point3 = p3;
        this.normal = new(0,0,0);
        CalcNormal();
        
    }
    public Triangle(){

        this.point1 = new Point(0,0,0);
        this.point2 = new Point(0,0,0);
        this.point3 = new Point(0,0,0);
        this.normal = new(0, 0, 0);
    }

    public Triangle Clone( ){
        return new Triangle(this.point1.Clone(),this.point2.Clone(),this.point3.Clone());
    }

    public void CalcNormal()
    {
        Point line1 = new(0, 0, 0);
        Point line2 = new(0, 0, 0);

        line1.x = this.point2.x - point1.x;
        line1.y = point2.y - point1.y;
        line1.z = point2.z - point1.z;

        line2.x = point3.x - point1.x;
        line2.y = point3.y - point1.y;
        line2.z = point3.z - point1.z;

        normal.x = line1.y * line2.z - line1.z * line2.y;
        normal.y = line1.z * line2.x - line1.x * line2.z;
        normal.z = line1.x * line2.y - line1.y * line2.x;

        float l = (float)Math.Sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        normal.x /= l; normal.y /= l; normal.z /= l;

    }
    
}


class Mesh{
    public Triangle[] triangles;

    public Mesh(Triangle[] triangles){
        if(triangles!= null){
            this.triangles = (Triangle[]?)triangles.Clone();
        }
    }
    public Mesh(){
        
    }

    public void AddTriangle(Triangle triangle){
        triangles.Append(triangle);
    }

    
}
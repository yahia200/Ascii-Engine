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

    public Triangle(Point p1, Point p2, Point p3){

        this.point1 = p1;
        this.point2 = p2;
        this.point3 = p3;
    }
    public Triangle(){

        this.point1 = new Point(0,0,0);
        this.point2 = new Point(0,0,0);
        this.point3 = new Point(0,0,0);
    }

    public Triangle Clone( ){
        return new Triangle(this.point1.Clone(),this.point2.Clone(),this.point3.Clone());
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
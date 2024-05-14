using System.Diagnostics;

class Screen{
    readonly int width;
    readonly int hight;
    Params param;
    public int show =160;

    int time = 0;

    public Screen(int w, int h){
        this.width = w;
        this.hight = h;
        ClearFrame();
        param = new(hight, width);
        FConsole.Initialize("Engine");
        FConsole.Clear();
        FConsole.FillBuffer(' ', (ConsoleColor)0, (ConsoleColor)0);

    }

    public void ClearFrame(){
        FConsole.Clear();
    }
    
    public void View(){
        FConsole.DrawBuffer();
    }

    public void White(){
        for (int h=0;h<hight;h++){
            for (int w=0;w<width;w++){
                FConsole.SetChar(w, h,'.', (ConsoleColor)0, 0);
            }
        }
    }

    public void DrawPoint(Point point){
        int x = (int)point.x;
        int y = (int)point.y;
        if( x < width && x >= 0 && y < hight && y >= 0){
        FConsole.SetChar((x), (y), '.',(ConsoleColor)15, 0);
        return;
        }
        throw new Exception("out of bound: (" + point.x + "," + point.y + ")");
    }

    private static void Swap<T>(ref T l, ref T r){
         T temp; temp = l; l = r; r = temp; 
    }

    public void DrawLine(Point point1, Point point2)
    {
        Point p1 = point1.Clone();
        Point p2 = point2.Clone();

        bool steep = Math.Abs(p2.y - p1.y) > Math.Abs(p2.x - p1.x);
        if (steep) { Swap(ref p1.x, ref p1.y); Swap(ref p2.x, ref p2.y); }
        if (p1.x > p2.x) { Swap(ref p1, ref p2);}
        int dX = (int)(p2.x - p1.x), dY = (int)Math.Abs(p2.y - p1.y), err = dX / 2, ystep = p1.y < p2.y ? 1 : -1, y = (int)p1.y;
        for (int x = (int)p1.x; x <= p2.x; ++x)
        {
            if(steep){DrawPoint(new Point(y,x,0));} 
            else{DrawPoint(new Point(x,y,0));}
            err -= dY;
            if (err < 0) { y += ystep;  err += dX; }
        }
    }

    public void DrawTriangle(Triangle triangle){
        DrawLine(triangle.point1, triangle.point2);
        DrawLine(triangle.point2, triangle.point3);
        DrawLine(triangle.point3, triangle.point1);
    }

    public void DrawMesh(Mesh mesh){
        param.SetRotZMatrix(0);
        param.SetRotXMatrix(1);
        param.SetRotYMatrix(time);
        foreach (Triangle triangle in mesh.triangles){

            Triangle rotTrix = RotaTrix(triangle);
            Triangle rotTrixy = RotaTriy(rotTrix);
            Triangle rotTrixyz = RotaTriz(rotTrixy);

            Triangle transTri = translate(rotTrixyz);

            Triangle projectedTri = ProjTrianlge(transTri);

            projectedTri.point1.x +=1;projectedTri.point1.y +=1;
            projectedTri.point2.x +=1;projectedTri.point2.y +=1;
            projectedTri.point3.x +=1;projectedTri.point3.y +=1;

            projectedTri.point1.x *= (int)(0.5*width); projectedTri.point1.y *= (int)(0.5*hight);

            projectedTri.point2.x *= (int)(0.5*width); projectedTri.point2.y *= (int)(0.5*hight);

            projectedTri.point3.x *= (int)(0.5*width); projectedTri.point3.y *= (int)(0.5*hight);


            // Console.WriteLine(triangle.point1.x + "," + triangle.point1.y + "," +  triangle.point1.z);
            // Console.WriteLine(projectedTri.point1.x + "," + projectedTri.point1.y + "," +  projectedTri.point1.z);
            // Console.WriteLine("----");
            // Console.WriteLine(triangle.point2.x + "," + triangle.point2.y + "," +  triangle.point2.z);
            // Console.WriteLine(projectedTri.point2.x + "," + projectedTri.point2.y + "," +  projectedTri.point2.z);
            // Console.WriteLine("----");
            // Console.WriteLine(triangle.point3.x + "," + triangle.point3.y + "," +  triangle.point3.z);
            // Console.WriteLine(projectedTri.point3.x + "," + projectedTri.point3.y + "," +  projectedTri.point3.z);
            // Console.WriteLine("-------------------------------------------------------------");
            DrawTriangle(projectedTri);
        }
        time++;
    }

    public Triangle translate(Triangle triangle)
    {
        Triangle transTri = triangle.Clone();
        transTri.point1.z += 3;
        transTri.point2.z += 3;
        transTri.point3.z += 3;
        return transTri;
    }


    public static Point MultiplyMatrixVector(Point i, float[,] m)
	{
        Point o = new(0, 0, 0)
        {
            x = (i.x * m[0, 0] + i.y * m[1, 0] + i.z * m[2, 0] + m[3, 0]),
            y = (i.x * m[0, 1] + i.y * m[1, 1] + i.z * m[2, 1] + m[3, 1]),
            z = (i.x * m[0, 2] + i.y * m[1, 2] + i.z * m[2, 2] + m[3, 2])
        };
        float w = (i.x * m[0,3] + i.y * m[1,3] + i.z * m[2,3] + m[3,3]);

		if (w != 0.0f)
		{
			o.x /= w; o.y /= w; o.z /= w;
		}

        return o;
	}

    public Triangle ProjTrianlge(Triangle triangle){
        Triangle t = new();
        t.point1 = MultiplyMatrixVector(triangle.point1, param.projMatrix);
        t.point2 = MultiplyMatrixVector(triangle.point2, param.projMatrix);
        t.point3 = MultiplyMatrixVector(triangle.point3, param.projMatrix);
        return t;
    }

    public Triangle RotaTrix(Triangle triangle){
        Triangle t = new ();
        t.point1 = MultiplyMatrixVector(triangle.point1, param.rotXMatrix);
        t.point2 = MultiplyMatrixVector(triangle.point2, param.rotXMatrix);
        t.point3 = MultiplyMatrixVector(triangle.point3, param.rotXMatrix);
        return t;

    }

    public Triangle RotaTriz(Triangle triangle){
        Triangle t = new ();
        //  param.SetRotZMatrix(60);
        t.point1 = MultiplyMatrixVector(triangle.point1, param.rotZMatrix);
        t.point2 = MultiplyMatrixVector(triangle.point2, param.rotZMatrix);
        t.point3 = MultiplyMatrixVector(triangle.point3, param.rotZMatrix);
        return t;

    } 
    public Triangle RotaTriy(Triangle triangle){
        Triangle t = new ();
        //  param.SetRotZMatrix(60);
        t.point1 = MultiplyMatrixVector(triangle.point1, param.rotYMatrix);
        t.point2 = MultiplyMatrixVector(triangle.point2, param.rotYMatrix);
        t.point3 = MultiplyMatrixVector(triangle.point3, param.rotYMatrix);
        return t;

    }

}

class Params{
    float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float fAspectRatio;
	float fFovRad;

    public float[,] projMatrix = new float[4,4];
    public float[,] rotXMatrix = new float[4,4];
    public float[,] rotZMatrix = new float[4,4];
    public float[,] rotYMatrix = new float[4,4];

    public Params(int hight, int width){
        fAspectRatio = (float)hight / (float)width;
        fFovRad = (float)(1.0f / Math.Tan(fFov * 0.5f / 180.0f * 3.14159f));
        SetProjMatrix();
    }

    public void SetProjMatrix(){
        projMatrix[0,0] = fAspectRatio * fFovRad;
		projMatrix[1,1] = fFovRad;
		projMatrix[2,2] = fFar / (fFar - fNear);
		projMatrix[3,2] = (-fFar * fNear) / (fFar - fNear);
		projMatrix[2,3] = 1.0f;
		projMatrix[3,3] = 0.0f;
    }

    public void SetRotXMatrix(float fTheta){
        rotXMatrix[0,0] = 1;
		rotXMatrix[1,1] = (float)Math.Cos(fTheta * 0.5f);
		rotXMatrix[1,2] = (float)Math.Sin(fTheta * 0.5f);
		rotXMatrix[2,1] = (float)- Math.Sin(fTheta * 0.5f);
		rotXMatrix[2,2] = (float)Math.Cos(fTheta * 0.5f);
		rotXMatrix[3,3] = 1;
    }

    public void SetRotZMatrix(float fTheta){
        rotZMatrix[0,0] = (float)Math.Cos(fTheta* 0.5f);
		rotZMatrix[0,1] = (float)Math.Sin(fTheta* 0.5f);
		rotZMatrix[1,0] = (float)-Math.Sin(fTheta* 0.5f);
		rotZMatrix[1,1] = (float)Math.Cos(fTheta* 0.5f);
		rotZMatrix[2,2] = 1;
		rotZMatrix[3,3] = 1;
    }
    public void SetRotYMatrix(float fTheta){
        rotYMatrix[0,0] = (float)Math.Cos(fTheta* 0.5f);
		rotYMatrix[0,2] = (float)-Math.Sin(fTheta* 0.5f);
        rotYMatrix[1, 1] = 1;
		rotYMatrix[2,0] = (float)Math.Sin(fTheta* 0.5f);
		rotYMatrix[2,2] = (float)Math.Cos(fTheta* 0.5f);
		rotYMatrix[3,3] = 1;
    }
}







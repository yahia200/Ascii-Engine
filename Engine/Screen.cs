using System.Diagnostics;

class Screen{
    readonly int width;
    readonly int hight;
    Params param;
    public int FT = 10;
    public Point camera = new(0,0,0);
    private string chars = " .:-=+*#%@";
    public int IntesityRes = 9;

    float time = 0;

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

    public void DrawPoint(Point point, int intesity){
        int x = (int)point.x;
        int y = (int)point.y;
        if( x < width && x >= 0 && y < hight && y >= 0){
        FConsole.SetChar((x), (y), chars[intesity],(ConsoleColor)15, 0);
        return;
        }
        throw new Exception("out of bound: (" + point.x + "," + point.y + ")");
    }

    private static void Swap<T>(ref T l, ref T r){
         T temp; temp = l; l = r; r = temp; 
    }

    public void DrawLine(Point point1, Point point2, int intesity)
    {
        Point p1 = point1.Clone();
        Point p2 = point2.Clone();

        bool steep = Math.Abs(p2.y - p1.y) > Math.Abs(p2.x - p1.x);
        if (steep) { Swap(ref p1.x, ref p1.y); Swap(ref p2.x, ref p2.y); }
        if (p1.x > p2.x) { Swap(ref p1, ref p2);}
        int dX = (int)(p2.x - p1.x), dY = (int)Math.Abs(p2.y - p1.y), err = dX / 2, ystep = p1.y < p2.y ? 1 : -1, y = (int)p1.y;
        for (int x = (int)p1.x; x <= p2.x; ++x)
        {
            if(steep){DrawPoint(new Point(y,x,0), intesity);} 
            else{DrawPoint(new Point(x,y,0), intesity);}
            err -= dY;
            if (err < 0) { y += ystep;  err += dX; }
        }
    }

    public List<int[]> FillLine(Point point1, Point point2, int intesity)
    {
        List<int[]> points = new List<int[]>();
        Point p1 = point1.Clone();
        Point p2 = point2.Clone();

        bool steep = Math.Abs(p2.y - p1.y) > Math.Abs(p2.x - p1.x);
        if (steep) { Swap(ref p1.x, ref p1.y); Swap(ref p2.x, ref p2.y); }
        if (p1.x > p2.x) { Swap(ref p1, ref p2);}
        int dX = (int)(p2.x - p1.x), dY = (int)Math.Abs(p2.y - p1.y), err = dX / 2, ystep = p1.y < p2.y ? 1 : -1, y = (int)p1.y;
        for (int x = (int)p1.x; x <= p2.x; ++x)
        {
            if(steep){DrawPoint(new Point(y,x,0), intesity); points.Add([y,x]); } 
            else{DrawPoint(new Point(x,y,0), intesity); points.Add([x, y]); }
            err -= dY;
            if (err < 0) { y += ystep;  err += dX; }
        }

        return points;
    }

    public void FillTriangle(Triangle triangle, int intensity)
    {
        List<int[]> Line1;
        List<int[]> Line2;
        List<int[]> Line3;
        Point p1 = triangle.point1.Clone();
        Point p2 = triangle.point2.Clone();
        Point p3 = triangle.point3.Clone();

        Line1 = FillLine(p1, p2, intensity);
        Line2 = FillLine(p1, p3, intensity);
        Line3 = FillLine(p2, p3, intensity);


        int i = 0;
        bool l1 = Line1.Count >= Line2.Count && Line1.Count >= Line3.Count;
        if (l1)
        {
            while (i < Line1.Count)
            {
                if (i < Line2.Count)
                {
                    DrawLine(new Point(Line1.ElementAt(i)[0], Line1.ElementAt(i)[1], 0), new Point(Line2.ElementAt(i)[0], Line2.ElementAt(i)[1], 0), intensity);
                    if (i < Line3.Count)
                        DrawLine(new Point(Line1.ElementAt(i)[0], Line1.ElementAt(i)[1], 0), new Point(Line3.ElementAt(i)[0], Line3.ElementAt(i)[1], 0), intensity);
                }

                else
                {

                    if (i < Line3.Count)
                        DrawLine(new Point(Line1.ElementAt(i)[0], Line1.ElementAt(i)[1], 0), new Point(Line3.ElementAt(i)[0], Line3.ElementAt(i)[1], 0), intensity);
                    else
                        break;
                }


                i++;
            }
            return;
        }


        bool l2 = Line2.Count >= Line1.Count && Line2.Count >= Line3.Count;
        if (l2)
        {
            while (i < Line2.Count)
            {
                if (i < Line1.Count)
                {
                    DrawLine(new Point(Line1.ElementAt(i)[0], Line1.ElementAt(i)[1], 0), new Point(Line2.ElementAt(i)[0], Line2.ElementAt(i)[1], 0), intensity);
                    if (i < Line3.Count)
                        DrawLine(new Point(Line2.ElementAt(i)[0], Line2.ElementAt(i)[1], 0), new Point(Line3.ElementAt(i)[0], Line3.ElementAt(i)[1], 0), intensity);
                }

                else
                {

                    if (i < Line3.Count)
                        DrawLine(new Point(Line2.ElementAt(i)[0], Line2.ElementAt(i)[1], 0), new Point(Line3.ElementAt(i)[0], Line3.ElementAt(i)[1], 0), intensity);
                    else
                        break;
                }


                i++;
            }
            return;
        }
        bool l3 = Line3.Count >= Line1.Count && Line3.Count >= Line2.Count;
        if (l3)
        {
            while (i < Line3.Count)
            {
                if (i < Line2.Count)
                {
                    DrawLine(new Point(Line3.ElementAt(i)[0], Line3.ElementAt(i)[1], 0), new Point(Line2.ElementAt(i)[0], Line2.ElementAt(i)[1], 0), intensity);
                    if (i < Line1.Count)
                        DrawLine(new Point(Line1.ElementAt(i)[0], Line1.ElementAt(i)[1], 0), new Point(Line3.ElementAt(i)[0], Line3.ElementAt(i)[1], 0), intensity);
                }

                else
                {

                    if (i < Line1.Count)
                        DrawLine(new Point(Line1.ElementAt(i)[0], Line1.ElementAt(i)[1], 0), new Point(Line3.ElementAt(i)[0], Line3.ElementAt(i)[1], 0), intensity);
                    else
                        break; 
                }


                i++;
            }
            return;
        }
        
    }



    public void DrawTriangle(Triangle triangle, int intsity){
            DrawLine(triangle.point1, triangle.point2, intsity);
            DrawLine(triangle.point2, triangle.point3, intsity);
            DrawLine(triangle.point3, triangle.point1, intsity);
            return;
        
        

    }

    public void DrawMesh(Mesh mesh, bool Fill = false){
        param.SetRotZMatrix(0);
        param.SetRotXMatrix(0);
        param.SetRotYMatrix(time/(FT*3));
        foreach (Triangle triangle in mesh.triangles){

            Triangle rotTrix = RotaTrix(triangle);
            Triangle rotTrixy = RotaTriy(rotTrix);
            Triangle rotTrixyz = RotaTriz(rotTrixy);

            Triangle transTri = translate(rotTrixyz);
            transTri.CalcNormal();

            if (transTri.normal.x * (transTri.point1.x - camera.x) +
               transTri.normal.y * (transTri.point1.y - camera.y) +
               transTri.normal.z * (transTri.point1.z - camera.z) < 0.0f)
            {
                Point light_direction = new(0, 0, -1);
                float l = (float)Math.Sqrt(light_direction.x * light_direction.x + light_direction.y * light_direction.y + light_direction.z * light_direction.z);
                light_direction.x /= l; light_direction.y /= l; light_direction.z /= l;
                float dp = transTri.normal.x * light_direction.x + transTri.normal.y * light_direction.y + transTri.normal.z * light_direction.z;
                int intesity = (int)(dp* IntesityRes);


                Triangle projectedTri = ProjTrianlge(transTri);

                projectedTri.point1.x += 1; projectedTri.point1.y += 1;
                projectedTri.point2.x += 1; projectedTri.point2.y += 1;
                projectedTri.point3.x += 1; projectedTri.point3.y += 1;

                projectedTri.point1.x *= (int)(0.5 * width); projectedTri.point1.y *= (int)(0.5 * hight);

                projectedTri.point2.x *= (int)(0.5 * width); projectedTri.point2.y *= (int)(0.5 * hight);

                projectedTri.point3.x *= (int)(0.5 * width); projectedTri.point3.y *= (int)(0.5 * hight);


                if (!Fill)
                {
                    DrawTriangle(projectedTri, intesity);
                }

                else
                {
                    FillTriangle(projectedTri, intesity);
                }
            }
            
        }
        time++;
        Thread.Sleep(FT);
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







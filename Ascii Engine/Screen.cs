class Screen{
    readonly int width;
    readonly int hight;
    readonly char[,] frame;

    public Screen(int w, int h){
        this.width = w;
        this.hight = h;
        frame = new char[h,w];
        ClearFrame();

    }

    public void ClearFrame(){
        for (int h=0;h<hight;h++){
            for (int w=0;w<width;w++){
                frame[h,w] = ' ';
            }
        }
    }
    
    public void View(){
        for (int h=0;h<hight;h++){
            for (int w=0;w<width;w++){
                Console.Write(frame[h,w]);
            }
            Console.WriteLine("");
        }
    }

    public void White(){
        for (int h=0;h<hight;h++){
            for (int w=0;w<width;w++){
                frame[h,w] = '.';
            }
        }
    }

    public bool DrawPoint(Point point){
        int x = point.x + width/2;
        int y = point.y + hight/2;
        frame[y,x] = '.';
        return true;
    }

    private static void Swap<T>(ref T l, ref T r){
         T temp; temp = l; l = r; r = temp; 
    }

        public void DrawLine(Point p1, Point p2)
        {
            bool steep = Math.Abs(p2.y - p1.y) > Math.Abs(p2.x - p1.x);
            if (steep) { Swap(ref p1.x, ref p1.y); Swap(ref p2.x, ref p2.y); }
            if (p1.x > p2.x) { Swap(ref p1, ref p2);}
            int dX = p2.x - p1.x, dY = Math.Abs(p2.y - p1.y), err = dX / 2, ystep = p1.y < p2.y ? 1 : -1, y = p1.y;

            for (int x = p1.x; x <= p2.x; ++x)
            {
                if(steep){DrawPoint(new Point(y,x,0));} 
                else{DrawPoint(new Point(x,y,0));}
                err -= dY;
                if (err < 0) { y += ystep;  err += dX; }
            }
        }
    
}



class Point{
    public int x;
    public int y;

    public int z;

    public Point(int x, int y, int z){
        this.x = x;
        this.y = y;
        this.z = z;
    }
}
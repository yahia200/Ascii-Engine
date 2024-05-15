using System.Threading;
// Console.WindowHeight = 120;
ConsoleHelper.SetCurrentFont("Consolas", 60);
Console.WindowWidth = 500;
Console.WindowHeight = 200;
Screen screen = new(Console.WindowWidth,Console.WindowHeight);
Point p1 = new(-1,-5,0);
Point p2 = new(-1,5,0);
Point p3 = new(-1,5,0);
Point p4 = new(-1,-5,0);
Triangle t1 = new(p1,p2,p3);
Triangle t2 = new(p2, p1, p4);
Triangle[] triangles = {t1, t2};
Mesh mesh = new(triangles);
// screen.DrawMesh(mesh);
screen.View();

Triangle[] cube = {

		
        new Triangle(new Point(0, 0, 0),    new Point(0, 1, 0),    new Point(1, 1, 0)),
        
		new Triangle(new Point(0, 0, 0),    new Point(1, 1, 0),    new Point(1, 0, 0 )),

		// EAST                                                      
		new Triangle(new Point(1, 0, 0),    new Point(1, 1, 0),    new Point(1, 1, 1)),
		new Triangle(new Point(1, 0, 0),    new Point(1, 1, 1),    new Point(1, 0, 1)),

		// NORTH                                                     
		new Triangle(new Point(1, 0, 1),    new Point(1, 1, 1),   new Point( 0, 1, 1)),
		new Triangle(new Point( 1, 0, 1),    new Point(0, 1, 1),    new Point(0, 0, 1)),

		// WEST                                                      
		new Triangle(new Point(0, 0, 1),    new Point(0, 1, 1),    new Point(0, 1, 0)),
		new Triangle( new Point(0, 0, 1),    new Point(0, 1, 0),    new Point(0, 0, 0)),

		// TOP                                                       
		new Triangle(new Point(0,1, 0),    new Point(0, 1, 1),    new Point(1, 1, 1)),
		new Triangle(new Point(0, 1, 0),    new Point(1, 1, 1),   new Point(1, 1, 0)),

		// BOTTOM                                                    
		new Triangle(new Point(1, 0, 1),    new Point(0, 0, 1),    new Point(0, 0, 0)),
		new Triangle(new Point(1, 0, 1),    new Point(0, 0, 0),    new Point(1, 0, 0 )),

		};

    Mesh mesh1 = new((Triangle[])cube.Clone());
#pragma warning disable CA1416 // Validate platform compatibility
Console.CursorSize = 1;
#pragma warning restore CA1416 // Validate platform compatibility

while (true)
{
	screen.DrawMesh(mesh1,Fill: true, Wire: true);
	screen.View();
	screen.ClearFrame();
}


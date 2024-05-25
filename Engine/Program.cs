ConsoleHelper.SetCurrentFont("Consolas", 10);
Console.WindowWidth = Console.LargestWindowWidth;
Console.WindowHeight = Console.LargestWindowHeight;
Screen screen = new(Console.WindowWidth,Console.WindowHeight);

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

Mesh mesh = new((Triangle[])cube.Clone());

while (true)
{
	screen.DrawMesh(mesh, Fill: true, Wire: false);
	screen.View();
	screen.ClearFrame();
}


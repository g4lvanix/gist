import java.util.ArrayList;
import java.util.ListIterator;
/**
 * Write a description of class testclass here.
 * 
 * @author (your name) 
 * @version (a version number or a date)
 */
public class testclass
{
    private ArrayList<Complex> numbers;
    private GaussPlane plane;
    
    public testclass()
    {
        numbers = new ArrayList<Complex>();
        //plane = new GaussPlane(6,6,1,1);
        
        
        numbers.add(new Complex(1,0));
        numbers.add(new Complex(0,1));
        numbers.add(new Complex(-1,0));
        numbers.add(new Complex(0, -1)); 
    }
    
    public void run()
    {
        
        ListIterator iter = numbers.listIterator();
        
        while(iter.hasNext())
        {
            Complex c = (Complex)iter.next();
            System.out.println("Rect: " + c.toString() + " Arg: " + c.arg() + " Mag^2: " + c.mag2());
        }
        /*
        for(int x = 0; x < 6; x++)
        {
        for(int y = 0; y < 6; y++)
        {
            Complex c = plane.getComplex(x,y);
            System.out.println(c.toString());
        }
        }*/
        
    }
    
    public static void main(String[] args)
    {
        testclass t = new testclass();
        
        t.run();
    }
}

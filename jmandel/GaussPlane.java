import java.util.ArrayList;

public class GaussPlane
{
    private ArrayList<ArrayList<Complex>> gplane;

    /**
    * Generates a representation/subset of the Gaussian (Complex) Plane
    * @param xticks     The number of x values in the subset
    * @param yticks     The number of y values in the subset
    * @param xrange     The range of x values in the subset
    * @param yrange     The range of y values in the subset
    */
    public GaussPlane(int xticks, int yticks, double xrange, double yrange)
    {
        gplane = new ArrayList<ArrayList<Complex>>();
        fillPlane(xticks,yticks,xrange,yrange);
    }

    /**
    * Gets the complex number at the specified x,y indices
    * @return z Complex number at position x,y
    */
    public Complex getComplex(int x, int y)
    {
        return gplane.get(x).get(y);
    }
    
    /**
    * Sets the complex number at the specified x,y indices
    * @param x  x coordinate
    * @param y  y coordinate
    * @paeam z  complex number 
    */
    public void setComplex(int x, int y, Complex z)
    {
        gplane.get(x).set(y,z);
    }

    /** 
    * Fill the subset of the Complex plane with the appropriate values
    * @param xticks     The number of x values in the subset
    * @param yticks     The number of y values in the subset
    * @param xrange     The range of x values in the subset
    * @param yrange     The range of y values in the subset
    */
    private void fillPlane(int xticks, int yticks, double xrange, double yrange)
    {
        double xinc = xrange/xticks;
        double yinc = yrange/yticks;

        double xstart = -1*(xrange/2);
        double ystart = -1*(yrange/2);

        double xt = xstart;
        double yt = ystart;

        for (int n = 0; n < xticks; n++)
        {
            gplane.add(new ArrayList<Complex>());
            for (int m = 0; m < yticks; m++)
            {
                Complex c = new Complex(xt,yt);
                gplane.get(n).add(m,c);
                yt += yinc;
            }
            yt = ystart;
            xt += xinc;
        }
    }
}
